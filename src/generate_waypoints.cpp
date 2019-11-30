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

    marker_x_pubs = n.advertise<std_msgs::Float64>(marker_x_topic, 1); //publish x-waypoint
    marker_y_pubs = n.advertise<std_msgs::Float64>(marker_y_topic,1); //publish y-waypoint

}

void mpcBlock::generate_waypoints::GetParams() {

    //Get parameters
    n.getParam("waypoint_filename", waypoint_filename);
    n.getParam("look_ahead_distance", look_ahead_distance);
    n.getParam("pose_topic", pose_topic);
    n.getParam("marker_x_topic", marker_x_topic);
    n.getParam("marker_y_topic", marker_y_topic);
}

void mpcBlock::generate_waypoints::pose_callback(const geometry_msgs::PoseStamped::ConstPtr &pose_msg) { //

    //const geometry_msgs::Pose pose_msg = odom_msg->pose.pose;
    double currentX = pose_msg->pose.position.x; //vehicle pose X
    double currentY = pose_msg->pose.position.y; //vehicle pose Y
    double currentTheta = mpcBlock::generate_waypoints::convert_to_Theta(pose_msg->pose.orientation); //vehicle orientation theta converted from quaternion to euler angle

    float waypoint_x;
    float waypoint_y;
    float distance_min = FLT_MAX;
    int ind_min = 0;

    for(int i = 0; i < waypoint_data1.size(); i++){
        float distance = sqrt(std::pow(currentX - waypoint_data1[i], 2) + std::pow(currentY - waypoint_data2[i], 2) );
        rot_waypoint_x = (waypoint_data1[i] - currentX) * cos(-currentTheta) - (waypoint_data2[i] - currentY) * sin(-currentTheta); //TODO: make it a rotation matrix

        if (distance_min > distance && distance >= look_ahead_distance && rot_waypoint_x > 0) {
            distance_min = distance;
            ind_min = i;
        }
    }

    last_index = ind_min;
    waypoint_x = waypoint_data1[last_index];
    waypoint_y = waypoint_data2[last_index];

    rot_waypoint_x = waypoint_x - currentX;
    rot_waypoint_y = waypoint_y - currentY;

    mpcBlock::generate_waypoints::rotate_points(currentTheta, &rot_waypoint_x, &rot_waypoint_y);

    chosen_waypoint_x.data = rot_waypoint_x;
    chosen_waypoint_y.data = rot_waypoint_y;
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
}

void mpcBlock::generate_waypoints::debug() { //Prints stuff on console for debugging

}

int main(int argc, char ** argv) {
    ros::init(argc, argv, "mpc_auto_node");
    mpcBlock::generate_waypoints wp_class_init;

    ros::Rate loop_rate(50);

    while(ros::ok()){
        wp_class_init.debug();
        wp_class_init.publisherCallback();

        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
