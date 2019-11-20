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
#include <cmath>
#include <Eigen/Geometry>
#include <Eigen/Dense>


namespace mpcBlock {

    #define Pi 3.142859

    struct laserdata{

            double angle_increment;
            double angle_sweep;

            int zero_angle; //0 degree
            int left_angle; //45 degree
            int right_angle; //45 degree

            std::vector<double> horizontal_scans = {};
            float y_upper_distance;
            float y_lower_distance;
            float y_mid_distance;
    };

    class predictor_class {

    public:
        predictor_class(); //Constructor

        void pose_callback(const geometry_msgs::PoseStamped::ConstPtr &pose_msg); //Subscribes to vehicle pose

        void lidar_callback(const sensor_msgs::LaserScan::ConstPtr &scan_msg);

        static double convert_to_Theta(geometry_msgs::Quaternion msg); //Converts vehicle orientation from quaternions to euler angles

        static  void rotate_points(double theta, float *distX, float *distY); //Brings WayPoints to the vehicle frame

        static double do_MPC(float waypoint_y, float waypoint_x, float y_upper, float y_lower); //Calls the MPC optimization routine to solve for optimal input

        void setAngleAndVelocity(double u); //Applies the derived input

    private:
        ros::NodeHandle n;
        ros::Publisher drive_pub; //publishes on the vehicle state
        ros::Publisher vis_pub; //For visualization stuff on RViz

        ros::Subscriber laser_sub; //subscribes to the laser scan
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

        double steering_angle = 0.0;
        double steering_limit;
        double high_velocity;
        double low_velocity;
        int last_index = -1;

        std::string waypoint_filename;
        std::string pose_topic;
        std::string drive_topic;
        std::string laser_topic;
        std::string visualization_topic;

        mpcBlock::laserdata current_scan; //struct to hold the laser data
    };

}