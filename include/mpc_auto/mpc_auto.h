#pragma once

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

namespace mpcBlock {

    #define Pi 3.142859

    class predictor_class {

    public:
        predictor_class(); //Constructor

        void pose_callback(const geometry_msgs::PoseStamped::ConstPtr &pose_msg); //Subscribes to vehicle pose

        double convert_to_Theta(const geometry_msgs::Quaternion msg); //Converts vehicle orientation from quaternions to euler angles

        void rotate_points(const double theta, float *distX, float *distY); //Brings WayPoints to the vehicle frame

        double do_MPC(const float waypoint_y, const float waypoint_x); //Calls the MPC optimization routine to solve for optimal input

        void setAngleAndVelocity(double u); //Applies the derived input

    private:
        ros::NodeHandle n;
        ros::Publisher drive_pub; //publishes on the vehicle state
        ros::Publisher vis_pub; //For visualization stuff on RViz
        ros::Subscriber pose_sub; //subscribes to the vehicle pose

        //WayPoint data containers
        std::vector <std::vector<float>> waypoint_data;
        std::vector <std::vector<float>> waypoint_data_long;
        std::vector<float> waypoint_data1;
        std::vector<float> waypoint_data2;
        std::vector<float> waypoint_data3;
        int waypoint_length = 0;
        float rot_waypoint_x = 0;
        float rot_waypoint_y = 0;

        visualization_msgs::Marker marker; //visualization stuff

        float look_ahead_distance = 0.7;
        float nominal_speed = 3;
        float angle_speed = 2;

        double steering_angle = 0.0;
        double steering_limit;
        double drive_velocity; //constant slow velocity model
        int last_index = -1;

        std::string waypoint_filename;
        std::string pose_topic;
        std::string drive_topic;
        std::string visualization_topic;
    };
}