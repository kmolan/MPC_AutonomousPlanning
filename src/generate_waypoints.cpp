/**Reads Waypoint CSV Files and generates optimum waypoint based on vehicle coordinates**/

#include "mpc_auto/read_waypoints.h"
#include "mpc_auto/generate_waypoints.h"

mpcBlock::generate_waypoints::generate_waypoints() {

    n = ros::NodeHandle(); //Initialize node

    GetParams();

    waypoint_data_full = mpcBlock::read_way_point_CSVfile(waypoint_filename);

    for (int i = 0; i < waypoint_data_full[0].size(); i++) {
        waypoint_data1.push_back(waypoint_data_full[0][i]);
        waypoint_data2.push_back(waypoint_data_full[1][i]);
    }

    localization_sub = n.subscribe(pose_topic, 1, &mpcBlock::generate_waypoints::pose_callback, this);
    ackermann_subs = n.subscribe(drive_topic, 1, &mpcBlock::generate_waypoints::ackermann_callback, this);

    marker_x_pubs = n.advertise<std_msgs::Float64>(marker_x_topic, 1); //publish x-waypoint
    marker_y_pubs = n.advertise<std_msgs::Float64>(marker_y_topic,1); //publish y-waypoint
    theta_pubs = n.advertise<std_msgs::Float64>(theta_topic, 1);

}

void mpcBlock::generate_waypoints::GetParams() {

    //Get parameters
    n.getParam("waypoint_filename", waypoint_filename);
    n.getParam("look_ahead_distance", look_ahead_distance);
    n.getParam("pose_topic", pose_topic);
    n.getParam("marker_x_topic", marker_x_topic);
    n.getParam("marker_y_topic", marker_y_topic);
    n.getParam("theta_topic", theta_topic);
    n.getParam("drive_topic", drive_topic);
}

void mpcBlock::generate_waypoints::updatePositions() {
    if(pf_update){
        prev_loop_time = current_loop_time;
        pf_update = false;
    }

    else {
        current_loop_time = ros::Time::now().toNSec();
    }

    currentApproxX = currentX + currentVelocity*std::cos(currentTheta)*(current_loop_time - prev_loop_time)/1000000000.0;
    currentApproxY = currentY + currentVelocity*std::sin(currentTheta)*(current_loop_time - prev_loop_time)/1000000000.0;

    float waypoint_x;
    float waypoint_y;
    float distance_min = FLT_MAX;
    int ind_min = 0;

    for(int i = 0; i < waypoint_data1.size(); i++){
        float distance = sqrt(std::pow(currentApproxX - waypoint_data1[i], 2) + std::pow(currentApproxY - waypoint_data2[i], 2) );
        rot_waypoint_x = (waypoint_data1[i] - currentApproxX) * cos(-currentTheta) - (waypoint_data2[i] - currentApproxY) * sin(-currentTheta); //TODO: make it a rotation matrix

        if (distance_min > distance && distance >= look_ahead_distance && rot_waypoint_x > 0) {
            distance_min = distance;
            ind_min = i;
        }
    }

    last_index = ind_min;
    waypoint_x = waypoint_data1[last_index];
    waypoint_y = waypoint_data2[last_index];

    prev_waypoint[0] = waypoint_data1[last_index-1];
    prev_waypoint[1] = waypoint_data2[last_index-1];

    next_waypoint[0] = waypoint_data1[last_index+1];
    next_waypoint[1] = waypoint_data2[last_index+1];

    rot_waypoint_x = waypoint_x - currentApproxX;
    rot_waypoint_y = waypoint_y - currentApproxY;

    float temp_prev_theta = std::atan2(waypoint_y - prev_waypoint[1], waypoint_x - prev_waypoint[0]);
    float temp_next_theta = std::atan2(next_waypoint[1] - waypoint_y, next_waypoint[0] - waypoint_x);

    mpcBlock::generate_waypoints::rotate_points(currentTheta, &rot_waypoint_x, &rot_waypoint_y);

    chosen_waypoint_x.data = rot_waypoint_x;
    chosen_waypoint_y.data = rot_waypoint_y;
    chosen_theta.data = currentTheta - (temp_prev_theta + temp_next_theta)/2.0;

    if(chosen_theta.data < -0.4189){
        chosen_theta.data = -0.4189;
    }

    if(chosen_theta.data > 0.4189){
        chosen_theta.data = 0.4189;
    }

    prev_loop_time = current_loop_time;
}

void mpcBlock::generate_waypoints::pose_callback(const nav_msgs::Odometry::ConstPtr &odom_msg) {

    pf_loop_time = ros::Time::now().toNSec();

    if(pf_loop_time - pf_last_loop_time > 0.02*1000000000.0) {
        const geometry_msgs::Pose pose_msg = odom_msg->pose.pose;
        currentX = pose_msg.position.x; //vehicle pose X
        currentY = pose_msg.position.y; //vehicle pose Y
        currentTheta = mpcBlock::generate_waypoints::convert_to_Theta(
                pose_msg.orientation); //vehicle orientation theta converted from quaternion to euler angle

        pf_update = true;
    }

    pf_last_loop_time = pf_loop_time;
}

void mpcBlock::generate_waypoints::ackermann_callback(const ackermann_msgs::AckermannDriveStamped::ConstPtr &ackermsg) {
    currentVelocity = ackermsg->drive.speed;
    currentSteering = ackermsg->drive.steering_angle;
}


double mpcBlock::generate_waypoints::convert_to_Theta(const geometry_msgs::Quaternion msg){
    // the incoming geometry_msgs::Quaternion is transformed to a tf::Quaternion
    tf::Quaternion quat;
    tf::quaternionMsgToTF(msg, quat);

    // the tf::Quaternion has a method to acess roll pitch and yaw
    double roll, pitch, yaw;
    tf::Matrix3x3(quat).getRPY(roll, pitch, yaw);

    //theta is just the yaw angle
    return yaw;
}

void mpcBlock::generate_waypoints::rotate_points(const double theta, float *distX, float *distY){

    Eigen::Matrix2f rotation_matrix2D;
    Eigen::MatrixXf dist_vector(2,1);

    dist_vector << *distX, *distY;
    rotation_matrix2D << std::cos(-theta), -std::sin(-theta), std::sin(-theta), std::cos(-theta);

    dist_vector = rotation_matrix2D*dist_vector;

    *distX = dist_vector(0,0);
    *distY = dist_vector(1,0);
}

void mpcBlock::generate_waypoints::publisherCallback() {
    marker_y_pubs.publish(chosen_waypoint_y);
    marker_x_pubs.publish(chosen_waypoint_x);
    theta_pubs.publish(chosen_theta);
}

void mpcBlock::generate_waypoints::debug() { //Prints stuff on console for debugging
//    ROS_INFO("chosen theta: %f", chosen_theta.data);
}

int main(int argc, char ** argv) {
    ros::init(argc, argv, "mpc_auto_node");
    mpcBlock::generate_waypoints wp_class_init;

    ros::Rate loop_rate(1000);

    while(ros::ok()){
        wp_class_init.updatePositions();
        wp_class_init.debug();
        wp_class_init.publisherCallback();

        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
