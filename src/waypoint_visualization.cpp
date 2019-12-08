/**Optional node for visualization of the waypoint**/

#include "mpc_auto/waypoint_visualization.h"
#include "mpc_auto/read_waypoints.h"

mpcBlock::visualizeWaypoint::visualizeWaypoint() {

    nh = ros::NodeHandle(); //initialize nodehandle

    getParams();

    vis_pub = nh.advertise<visualization_msgs::Marker>(visualization_topic, 0);

    waypoint_index_subs = nh.subscribe(waypoint_index_topic, 10, &mpcBlock::visualizeWaypoint::waypoint_index_callback, this);

    waypoint_data_full = mpcBlock::read_way_point_CSVfile(waypoint_filename);

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
    nh.getParam("waypoint_index_topic", waypoint_index_topic);
    nh.getParam("waypoint_filename", waypoint_filename);
}

void mpcBlock::visualizeWaypoint::waypoint_index_callback(const std_msgs::Float64::ConstPtr &msg) {
    marker_x = waypoint_data_full[0][msg->data];
    marker_y = waypoint_data_full[1][msg->data];
}

void mpcBlock::visualizeWaypoint::publisherCallback() {
    marker.header.frame_id = "map";
    marker.pose.position.x = marker_x;
    marker.pose.position.y = marker_y;
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


