#pragma once

using namespace std;

#include <vector>
#include <iterator>
#include <string>
#include <fstream>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <sensor_msgs/LaserScan.h>
#include <ackermann_msgs/AckermannDriveStamped.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Quaternion.h>
#include <nav_msgs/Odometry.h>
#include <visualization_msgs/Marker.h>
#include <tf/transform_datatypes.h>
#include <math.h>
#include <Eigen/Geometry>
#include <Eigen/Dense>

#define Pi 3.142859

class mpcc{

    public:
        mpcc();

        void pose_callback(const nav_msgs::Odometry::ConstPtr &odom_msg);

        double convert_to_Theta(const geometry_msgs::Quaternion msg);

        void rotate_points(const double theta, float* distX, float* distY);

        double do_MPC(const float waypoint_x, const float waypoint_y, const double currentX, const double currentY);

        void setAngleAndVelocity(double u);

    private:
        ros::NodeHandle n;
        ros::Publisher drive_pub;
        ros::Publisher vis_pub;
        ros::Subscriber pose_sub;

        vector<vector<float>> waypoint_data;
        vector<vector<float>> waypoint_data_long;
        vector<float> waypoint_data1;
        vector<float> waypoint_data2;
        vector<float> waypoint_data3;

        visualization_msgs::Marker marker;

        float angle_factor = 0.1;
        float look_ahead_distance = 0.7;
        float nominal_speed = 3;
        float angle_speed = 2;
        int first_time = 1;

        double steering_angle = 0.0;
        int last_index = -1;
        float last_distance = 0;
        int waypoint_length = 0;
        float rot_waypoint_x = 0;
        float rot_waypoint_y = 0;
};