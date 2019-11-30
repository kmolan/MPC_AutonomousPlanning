/**Optional node for visualization of the waypoint**/

#include "mpc_auto/waypoint_visualization.h"

mpcBlock::visualizeWaypoint::visualizeWaypoint() {

    nh = ros::NodeHandle(); //initialize nodehandle

    getParams();

    vis_pub = nh.advertise<visualization_msgs::Marker>(visualization_topic, 0);
    marker_x_subs = nh.subscribe(marker_x_topic, 1, &mpcBlock::visualizeWaypoint::marker_x_callback, this);
    marker_y_subs = nh.subscribe(marker_y_topic, 1, &mpcBlock::visualizeWaypoint::marker_y_callback, this);

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

void mpcBlock::visualizeWaypoint::getParams() {
    nh.getParam("visualization_topic", visualization_topic);
    nh.getParam("marker_x_topic", marker_x_topic);
    nh.getParam("marker_y_topic", marker_y_topic);
}

void mpcBlock::visualizeWaypoint::marker_x_callback(const std_msgs::Float64::ConstPtr &msg) {
    marker_x.data = msg->data;
}

void mpcBlock::visualizeWaypoint::marker_y_callback(const std_msgs::Float64::ConstPtr &msg) {
    marker_y.data = msg->data;
}

void mpcBlock::visualizeWaypoint::publisherCallback() {

    marker.header.frame_id = "map";
    marker.pose.position.x = marker_x.data;
    marker.pose.position.y = marker_y.data;
    marker.id += 1;
    marker.header.stamp = ros::Time();
    marker.lifetime = ros::Duration(0.1);
    vis_pub.publish(marker);
}

void mpcBlock::visualizeWaypoint::debug() {

}

int main(int argc, char ** argv) {
    ros::init(argc, argv, "waypoint_visualization");
    mpcBlock::visualizeWaypoint wv_class_init;

    ros::Rate loop_rate(50);

    while(ros::ok()){
        wv_class_init.publisherCallback();
        wv_class_init.debug();
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}


