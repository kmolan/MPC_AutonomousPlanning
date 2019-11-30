#include "mpc_auto/doMPC.h"

Vars vars;
Params params;
Workspace work;
Settings settings;

#include "mpc_auto/cvxgen_mpc.h"

mpcBlock::doMPC::doMPC() {

    nodeH = ros::NodeHandle(); //Initialize node

    getParams();

    lidar_sub = nodeH.subscribe(laser_topic, 1, &mpcBlock::doMPC::lidar_callback, this);

    drive_pub = nodeH.advertise<ackermann_msgs::AckermannDriveStamped>(drive_topic, 1); //publishes steering angle and velocity

    marker_x_subs = nodeH.subscribe(marker_x_topic, 1, &mpcBlock::doMPC::marker_x_callback, this); //publish x-waypoint
    marker_y_subs = nodeH.subscribe(marker_y_topic, 1, &mpcBlock::doMPC::marker_y_callback, this); //publish y-waypoint

    current_scan.angle_increment = 0.00582316;
    current_scan.angle_sweep = 6.28319;
    current_scan.zero_angle = int(0.5*current_scan.angle_sweep/current_scan.angle_increment); //Calculate index for left side 0 degree
    current_scan.left_angle = int(0.75*current_scan.angle_sweep/current_scan.angle_increment); //Calculate index for left side 90 degree
    current_scan.right_angle = int(0.25*current_scan.angle_sweep/current_scan.angle_increment); //Calculate index for 90 degree angle

    current_scan.y_lower_distance = -1;
    current_scan.y_upper_distance = 1;
}

void mpcBlock::doMPC::getParams() {

    //Get parameters
    nodeH.getParam("laser_topic", laser_topic);
    nodeH.getParam("drive_topic", drive_topic);
    nodeH.getParam("marker_x_topic", marker_x_topic);
    nodeH.getParam("marker_y_topic", marker_y_topic);
    nodeH.getParam("steering_limit", steering_limit);
    nodeH.getParam("high_velocity", high_velocity);
    nodeH.getParam("low_velocity", low_velocity);
    nodeH.getParam("lower_threshold", lower_threshold);
    nodeH.getParam("midline_threshold", midline_threshold);
    nodeH.getParam("breakneck_steering", breakneck_steering);
    nodeH.getParam("min_halfspace_width",min_halfspace_width);
    nodeH.getParam("breakneck_steering_threshold", breakneck_steering_threshold);
    nodeH.getParam("Q_matrix_1", Q_matrix_1);
    nodeH.getParam("Q_matrix_2" ,Q_matrix_2);
    nodeH.getParam("R_matrix_1", R_matrix_1);
    nodeH.getParam("B_matrix", B_matrix);
}

void mpcBlock::doMPC::marker_x_callback(const std_msgs::Float64::ConstPtr &msg) {
    rot_waypoint_x = msg->data;
}

void mpcBlock::doMPC::marker_y_callback(const std_msgs::Float64::ConstPtr &msg) {
    rot_waypoint_y = msg->data;
}

void mpcBlock::doMPC::lidar_callback(const sensor_msgs::LaserScan::ConstPtr &scan_msg){

    int x_lower_index = current_scan.right_angle;
    int x_upper_index = current_scan.left_angle;

    for(int i = current_scan.zero_angle; i>=current_scan.right_angle; i--){
        if(scan_msg->ranges[i] < lower_threshold){
            x_lower_index = i;
            break;
        }
    }

    for(int i = current_scan.zero_angle; i<=current_scan.left_angle; i++){
        if(scan_msg->ranges[i] < lower_threshold){
            x_upper_index = i;
            break;
        }
    }

    current_scan.y_lower_distance = scan_msg->ranges[x_lower_index]*std::sin(-Pi + 2*Pi*float(x_lower_index)/float(scan_msg->ranges.size())) ;
    current_scan.y_upper_distance =  scan_msg->ranges[x_upper_index-1]*std::sin(-Pi + 2*Pi*float(x_upper_index)/float(scan_msg->ranges.size())) ;

    if(current_scan.y_lower_distance > -min_halfspace_width){
        current_scan.y_lower_distance = 0;
    }

    if(current_scan.y_upper_distance < min_halfspace_width){
        current_scan.y_upper_distance = 0;
    }

    current_scan.y_mid_distance = (current_scan.y_lower_distance + current_scan.y_upper_distance)/2.0;

    if(current_scan.y_lower_distance > -breakneck_steering_threshold && current_scan.y_upper_distance < breakneck_steering_threshold){
        current_scan.y_mid_distance = breakneck_steering;
    }

    if(current_scan.y_mid_distance > midline_threshold){
        current_scan.y_mid_distance = 0;
    }

    rot_waypoint_y = rot_waypoint_y + current_scan.y_mid_distance; //offset by midline

    mpcBlock::doMPC::controller_callback();

    ROS_INFO("steering angle: %f", steering_angle);
}

void mpcBlock::doMPC::controller_callback() {

    (run_cvxgenOptimization(Q_matrix_1, Q_matrix_2, R_matrix_1, B_matrix));
    steering_angle = run_cvxgenOptimization::solve_mpc(rot_waypoint_y, rot_waypoint_x);

}

void mpcBlock::doMPC::publisherCallback() {

    ackermann_msgs::AckermannDriveStamped drive_msg;
    drive_msg.drive.steering_angle = steering_angle; //Sets steering angle
    drive_msg.drive.speed = high_velocity - (high_velocity - low_velocity) * std::abs(steering_angle) / steering_limit; //Interpolation function
    drive_pub.publish(drive_msg); //Sets velocity based on steering angle conditions

}

void mpcBlock::doMPC::debug() { //Prints stuff on console for debugging. Commented out code is temporarily not being debugged

        std::cout << "upper: " << current_scan.y_upper_distance << std::endl;
        std::cout << "lower: " << current_scan.y_lower_distance << std::endl;
        std::cout << "mid: " << current_scan.y_mid_distance << std::endl;
}

int main(int argc, char ** argv) {
    ros::init(argc, argv, "doMPC");
    mpcBlock::doMPC mpc_class_init;

    ros::Rate loop_rate(50);

    while(ros::ok()){

        mpc_class_init.debug();
        mpc_class_init.publisherCallback();

        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
