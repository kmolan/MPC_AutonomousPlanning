#pragma once

#include <vector>
#include <iterator>
#include <string>
#include <std_msgs/Float64.h>
#include <fstream>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Quaternion.h>
#include <nav_msgs/Odometry.h>
#include <visualization_msgs/Marker.h>
#include <tf/transform_datatypes.h>
#include <cmath>
#include <Eigen/Geometry>
#include <Eigen/Dense>
#include <ackermann_msgs/AckermannDriveStamped.h>
#include "cvxgen_mpc.h"

namespace mpcBlock {

    class generate_waypoints {

    public:
        /*!
         * @brief default constructor for the predictor class
         */
        generate_waypoints();

        /*!
         * @brief calls all the class publishers
         */
        void publisherCallback();

        /*!
         * @brief print stuff on console for debugging
         */
        void debug();

    private:

        /*!
         * @brief subscribes to the pose of the vehicle using particle filter
         * @param pose_msg container pointer for the vehicle pose
         */
        void pose_callback(const nav_msgs::Odometry::ConstPtr &odom_msg);

        /*!
         * @brief fetches parameters and assigns it to member variables
         */
        void GetParams();

        /*!
         * @brief converts quaternions to euler angles
         * @param msg incoming quaternion
         * @return yaw angle (heading direction of the vehicle)
         */
        static double convert_to_Theta(geometry_msgs::Quaternion msg);

        /*!
         * @brief Brings waypoints to the vehicle frame
         * @param theta [in] yaw angle of the vehicle (heading angle)
         * @param distX [out] X-direction of the waypoint
         * @param distY [out] Y-direction of the waypoint
         */
        static void rotate_points(double theta, float *distX, float *distY);

        ros::NodeHandle n; ///< NodeHandle
        ros::Publisher marker_x_pubs; ///<publishes the current waypoint, x-coordinate
        ros::Publisher marker_y_pubs; ///<publishes the current waypoint, y-coordinate
        ros::Publisher waypoint_index_pubs; ///<publishes index of the chosen waypoints

        ros::Subscriber localization_sub; ///<  subscribes to the particle filter for localization

        //WayPoint data containers
        std::vector <std::vector<float>> waypoint_data_full; ///< Original raw waypoint data
        std::vector<float> waypoint_data1; ///<Processed waypoint data in y-direction
        std::vector<float> waypoint_data2;///<Processed waypoint data in x-direction
        float rot_waypoint_x = 0; ///<optimal waypoint in vehicle frame, x-direction
        float rot_waypoint_y = 0; ///<optimal waypoint in vehicle frame, y-direction

        float look_ahead_distance; ///<Look-ahead parameter for choosing next optimal waypoint

        std::string marker_x_topic; ///<Topic over which marker_x_pubs publishes
        std::string marker_y_topic; ///<Topic over which marker_y_pubs publishes
        std::string waypoint_index_topic; ///<Topic over which waypoint_index_pubs publishes
        std_msgs::Float64 chosen_waypoint_x; ///<Container for current waypoint x-coordinate
        std_msgs::Float64 chosen_waypoint_y; ///<Container for current waypoint y-coordinate
        std_msgs::Float64 waypoint_index_msg; ///<Container for current waypoint index

        double currentX; ///<Current X-coordinate from particle filter
        double currentY; ///<Current Y-coordinate from particle filter
        double currentTheta; ///<Current Heading angle from particle filter

        int last_index = -1; ///<Index of the selected optimal waypoint

        std::string waypoint_filename; ///<Address of the CSV file containing waypoints
        std::string pose_topic; ///< subscriber topic, particle filter publishes poses on this
    };

}
