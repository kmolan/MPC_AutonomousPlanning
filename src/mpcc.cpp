//getparam file
//consistent data types, not everything double!

#include "mpcc/read_way_point_CSVfile.h"
#include "mpcc/mpcc.h"
//#include"solver.h"

mpcBlock::predictor_class::predictor_class() {

    n = ros::NodeHandle();

    // load the WayPoints and simplify them
    waypoint_data_long = mpcBlock::read_way_point_CSVfile("/home/anmolk/ese680_ws/src/gtpose.csv");

    waypoint_length = waypoint_data_long[0].size();

    for (int i = 0; i < waypoint_length; i+=250) {
        waypoint_data1.push_back(waypoint_data_long[0][i]);
        waypoint_data2.push_back(waypoint_data_long[1][i]);
        waypoint_data3.push_back(waypoint_data_long[2][i]);
    }

    waypoint_data.push_back(waypoint_data1);
    waypoint_data.push_back(waypoint_data2);
    waypoint_data.push_back(waypoint_data3);
    waypoint_length = waypoint_data[0].size();

    pose_sub = n.subscribe("pf/pose/odom", 1, &mpcBlock::predictor_class::pose_callback, this);
    drive_pub = n.advertise<ackermann_msgs::AckermannDriveStamped>("drive", 1); //publishes steering angle and velocity
    vis_pub = n.advertise<visualization_msgs::Marker>( "visualization_marker", 0 );

    marker.header.frame_id = "map";
    marker.ns = "my_namespace";
    marker.type = visualization_msgs::Marker::SPHERE;
    marker.action = visualization_msgs::Marker::ADD;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;
    marker.scale.x = 0.2;
    marker.scale.y = 0.2;
    marker.scale.z = 0.1;
    marker.color.a = 1.0;
    marker.color.r = 1.0;
    marker.color.g = 0.0;
    marker.color.b = 0.0;
    marker.id = 0;
}

void mpcBlock::predictor_class::pose_callback(const nav_msgs::Odometry::ConstPtr &odom_msg) {

    const geometry_msgs::Pose pose_msg = odom_msg->pose.pose;
    double currentX = pose_msg.position.x; //vehicle pose X
    double currentY = pose_msg.position.y; //vehicle pose Y
    double currentTheta = mpcBlock::predictor_class::convert_to_Theta(pose_msg.orientation); //vehicle orientation theta converted from quaternion to euler angle

    float waypoint_x;
    float waypoint_y;
    float waypoint_distance = sqrt( std::pow(currentX - waypoint_x, 2) + std::pow(currentY - waypoint_y, 2) );
    float distance_min = FLT_MAX;
    float ind_min = 0;

    for(int i = 0; i < waypoint_length; i ++){
        float distance = sqrt( std::pow(currentX - waypoint_data[0][i], 2) + std::pow(currentY - waypoint_data[1][i], 2) );
        rot_waypoint_x = (waypoint_data[0][i] - currentX) * cos(-currentTheta) - (waypoint_data[1][i] - currentY) * sin(-currentTheta); //anmol: make it a rotation matrix

        if (distance_min > distance && distance >= look_ahead_distance && rot_waypoint_x > 0) {
            distance_min = distance;
            ind_min = i;
        }
    }

    last_index = ind_min;
    waypoint_x = waypoint_data[0][last_index];
    waypoint_y = waypoint_data[1][last_index];

    rot_waypoint_x = waypoint_x - currentX;
    rot_waypoint_y = waypoint_y - currentY;

    mpcBlock::predictor_class::rotate_points(currentTheta, &rot_waypoint_x, &rot_waypoint_y);

    steering_angle = mpcBlock::predictor_class::do_MPC(rot_waypoint_x, rot_waypoint_y, currentX, currentY);

    marker.header.frame_id = "map";
    marker.pose.position.x = waypoint_x;
    marker.pose.position.y = waypoint_y;
    marker.color.r = 1.0;
    marker.color.g = 0.0;
    marker.color.b = 0.0;
    marker.id += 1;
    marker.header.stamp = ros::Time();
    marker.lifetime = ros::Duration(0.1);
    vis_pub.publish(marker);

    // marker.header.frame_id = "laser";
    // marker.pose.position.x = rot_waypoint_x;
    // marker.pose.position.y = rot_waypoint_y;
    // marker.color.r = 0.0;
    // marker.color.g = 0.0;
    // marker.color.b = 1.0;
    // marker.id += 1;
    // marker.header.stamp = ros::Time();
    // marker.lifetime = ros::Duration(0.1);
    // vis_pub.publish(marker);
    // }

    mpcBlock::predictor_class::setAngleAndVelocity(steering_angle);
}

double mpcBlock::predictor_class::convert_to_Theta(const geometry_msgs::Quaternion msg){
    // the incoming geometry_msgs::Quaternion is transformed to a tf::Quaterion
    tf::Quaternion quat;
    tf::quaternionMsgToTF(msg, quat);

    // the tf::Quaternion has a method to acess roll pitch and yaw
    double roll, pitch, yaw;
    tf::Matrix3x3(quat).getRPY(roll, pitch, yaw);

    //theta is just the yaw angle
    return yaw;
}

void mpcBlock::predictor_class::rotate_points(const double theta, float *distX, float *distY){

    Eigen::Matrix2f rotation_matrix2D;
    Eigen::MatrixXf dist_vector(2,1);

    dist_vector << *distX, *distY;
    rotation_matrix2D << std::cos(-theta), -std::sin(-theta), std::sin(-theta), std::cos(-theta);

    dist_vector = rotation_matrix2D*dist_vector;

    *distX = dist_vector(0,0);
    *distY = dist_vector(1,0);
}

double mpcBlock::predictor_class::do_MPC(const float waypoint_x, const float waypoint_y, const double currentX, const double currentY){

    /* set_defaults();
     * setup_indexing();
     *
     * optimizer::params.A[0] = 1; ......
     * solve();
     * if(work.converged){
     * conditions.. ROS_ERROR
     * }
     * return vars.u_10[0]
     */
    double steering_angle = 0;
    return steering_angle;
}

void mpcBlock::predictor_class::setAngleAndVelocity(double u) {

    ackermann_msgs::AckermannDriveStamped drive_msg;
    double steering_limit;
    steering_limit = 0.4189;

    if(u < -steering_limit){
        u = -steering_limit;
    }
    if(u > steering_limit){
        u = steering_limit;
    }

    drive_msg.drive.steering_angle = u; //Sets steering angle
    //drive_msg.drive.speed = nominal_speed - (nominal_speed - angle_speed) * fabs(u) / 0.4189;
    drive_msg.drive.speed = 2; //constant slow velocity model
    drive_pub.publish(drive_msg); //Sets velocity based on steering angle conditions

}

int main(int argc, char ** argv) {
    ros::init(argc, argv, "mpc node");
    mpcBlock::predictor_class pp;
    ros::spin();
    return 0;
}
