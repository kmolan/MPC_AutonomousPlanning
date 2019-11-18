//consistent data types, not everything double!

#include "mpc_auto/read_way_point_CSVfile.h"
#include "mpc_auto/mpc_auto.h"
#include "cvxgen/solver.h"

Vars vars;
Params params;
Workspace work;
Settings settings;

#include "mpc_auto/cvxgen_mpc.h"

mpcBlock::predictor_class::predictor_class() {

    n = ros::NodeHandle(); //Initialize node

    //Get parameters
    n.getParam("waypoint_filename", waypoint_filename);
    n.getParam("pose_topic", pose_topic);
    n.getParam("laser_topic", laser_topic);
    n.getParam("drive_topic", drive_topic);
    n.getParam("visualization_topic", visualization_topic);
    n.getParam("steering_limit", steering_limit);
    n.getParam("high_velocity", high_velocity);
    n.getParam("low_velocity", low_velocity);

    waypoint_data_long = mpcBlock::read_way_point_CSVfile(waypoint_filename);

    waypoint_length = waypoint_data_long[0].size();

    for (int i = 0; i < waypoint_length; i++) { //250 is the sample length
        waypoint_data1.push_back(waypoint_data_long[0][i]);
        waypoint_data2.push_back(waypoint_data_long[1][i]);
        waypoint_data3.push_back(waypoint_data_long[2][i]);
    }

    waypoint_data.push_back(waypoint_data1);
    waypoint_data.push_back(waypoint_data2);
    waypoint_data.push_back(waypoint_data3);
    waypoint_length = waypoint_data[0].size();

    pose_sub = n.subscribe(pose_topic, 1, &mpcBlock::predictor_class::pose_callback, this);
    laser_sub = n.subscribe(laser_topic, 1, &mpcBlock::predictor_class::lidar_callback, this);

    drive_pub = n.advertise<ackermann_msgs::AckermannDriveStamped>(drive_topic, 1); //publishes steering angle and velocity
    vis_pub = n.advertise<visualization_msgs::Marker>(visualization_topic, 0);

    current_scan.angle_increment = 0.00582316;
    current_scan.angle_sweep = 6.28319;
    current_scan.zero_angle = round(0.5*current_scan.angle_sweep/current_scan.angle_increment); //Calculate index for zero angle
    current_scan.left_angle = round(0.6666*current_scan.angle_sweep/current_scan.angle_increment); //Calculate index for left side 22.5 degree
    current_scan.right_angle = round(0.5625*current_scan.angle_sweep/current_scan.angle_increment); //Calculate index for 22.5 degree angle

    //visualization stuff
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

void mpcBlock::predictor_class::pose_callback(const geometry_msgs::PoseStamped::ConstPtr &pose_msg) { //

    //const geometry_msgs::Pose pose_msg = odom_msg->pose.pose;
    double currentX = pose_msg->pose.position.x; //vehicle pose X
    double currentY = pose_msg->pose.position.y; //vehicle pose Y
    double currentTheta = mpcBlock::predictor_class::convert_to_Theta(pose_msg->pose.orientation); //vehicle orientation theta converted from quaternion to euler angle
    std::cout<< "current theta" << currentTheta << std::endl;

    float waypoint_x;
    float waypoint_y;
    float distance_min = FLT_MAX;
    float ind_min = 0;

    for(int i = 0; i < waypoint_length; i++){
        float distance = sqrt(std::pow(currentX - waypoint_data[0][i], 2) + std::pow(currentY - waypoint_data[1][i], 2) );
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

    steering_angle = mpcBlock::predictor_class::do_MPC(rot_waypoint_y, rot_waypoint_x);

    //std::cout << "steering angle: "<< steering_angle << std::endl;

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

    mpcBlock::predictor_class::setAngleAndVelocity(steering_angle);
}


double mpcBlock::predictor_class::convert_to_Theta(const geometry_msgs::Quaternion msg){
    // the incoming geometry_msgs::Quaternion is transformed to a tf::Quaternion
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


void mpcBlock::predictor_class::lidar_callback(const sensor_msgs::LaserScan::ConstPtr &scan_msg){

    std::vector<double> temp_vector;

    for(int i = current_scan.right_angle; i<current_scan.left_angle; i++){
        temp_vector.push_back(scan_msg->ranges[i]);
    }

    int max_count = INT_MIN;
    int start_index = 0;
    int i = 0;
    float lower_threshold = 1.0;
    int x_lower_limit = 0;
    int x_upper_limit = 0;

    while(i < temp_vector.size()){
        int count=0;

        if(temp_vector[i] >= lower_threshold){
            start_index = i;
            int j = i;

            while(j<temp_vector.size()){

                if(temp_vector[j] > lower_threshold){
                    count++;
                }

                else{
                    break;
                }

                j++;
            }

            if(count > max_count){
                max_count = count;
                x_lower_limit = start_index;
                x_upper_limit = j;
            }

            i = start_index+1;
        }

        else{
            i++;
        }
    }

/*    std::cout << "start index: " << x_lower_limit << std::endl;
    std::cout << "end index: " << x_upper_limit << std::endl;
    std::cout<< "size is: " << temp_vector.size() << std::endl;*/
}

double mpcBlock::predictor_class::do_MPC(const float waypoint_y, const float waypoint_x){

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
    run_cvxgenOptimization temp_class;

    return temp_class.solve_mpc(waypoint_y, waypoint_x);
}

void mpcBlock::predictor_class::setAngleAndVelocity(double u) {

    ackermann_msgs::AckermannDriveStamped drive_msg;
    drive_msg.drive.steering_angle = u; //Sets steering angle
    drive_msg.drive.speed = high_velocity - (high_velocity - low_velocity) * fabs(u) / steering_limit; //Zirui's magical interpolation function
    drive_pub.publish(drive_msg); //Sets velocity based on steering angle conditions

}

int main(int argc, char ** argv) {
    ros::init(argc, argv, "mpc_auto_node");
    mpcBlock::predictor_class pp;
    ros::spin();
    return 0;
}
