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
#include "cvxgen_mpc.h"


namespace mpcBlock {

    #define Pi 3.142859

    /*!
     * @brief struct that holds the incoming lidar data
     * @param angle_increment the angle between two consecutive readings
     * @param angle_sweep the total sweep of angle for lidar readings
     * @param zero_angle the index for zero angle (straight ahead) in lidar readings
     * @param left_angle the index for +90 degrees in lidar readings (left of the vehicle)
     * @param right_angle the index for -90 degrees in lidar readings (right of the vehicle)
     * @param y_upper_distance the y-axis distance from the vehicle towards closest obstacle at left side of the car
     * @param y_lower_distance the y-axis distance from the vehicle towards closest obstacle at right side of the car
     * @param y_mid_distance the mean of upper and lower distances
     */
    struct laserdata{

            double angle_increment;
            double angle_sweep;

            int zero_angle; //0 degree
            int left_angle; //90 degree
            int right_angle; //90 degree

            float y_upper_distance;
            float y_lower_distance;
            float y_mid_distance;
    };

    class predictor_class {

    public:
        /*!
         * @brief default constructor for the predictor class
         */
        predictor_class();

        /*!
         * @brief subscribes to the pose of the vehicle using particle filter
         * @param pose_msg container pointer for the vehicle pose
         */
        void pose_callback(const geometry_msgs::PoseStamped::ConstPtr &pose_msg);

        /*!
         * @brief subscribes to the lidar readings for obstacle avoidance
         * @param scan_msg container pointer for the lidar readings
         */
        void lidar_callback(const sensor_msgs::LaserScan::ConstPtr &scan_msg);

        /*!
         * @brief print stuff on console for debugging
         */
        void debug();

    private:

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

        /*!
         * @brief Calls the MPC obtimization routine to solve for optimal input
         * @param waypoint_y Y-coordinate of chosen waypoint
         * @param waypoint_x X-coordinate of chosen waypoint
         * @return optimal steering angle
         */
        double do_MPC(float waypoint_y, float waypoint_x);

        /*!
         * @brief applies the optimal input to the vehicle
         * @param u optimal steering angle derived from MPC
         */
        void setAngleAndVelocity(double u);

        ros::NodeHandle n; ///< NodeHandle
        ros::Publisher drive_pub; ///< publishes on vehicle inputs (steering and velocity)
        ros::Publisher vis_pub; ///< For visualization stuff on RViz

        ros::Subscriber lidar_sub; ///< subscribes to the lidar scan for obstacle avoidance
        ros::Subscriber localization_sub; ///<  subscribes to the particle filter for localization

        //WayPoint data containers
        std::vector <std::vector<float>> waypoint_data_full; ///< Original raw waypoint data
        std::vector<float> waypoint_data1; ///<Processed waypoint data in y-direction //TODO: Check axes
        std::vector<float> waypoint_data2;///<Processed waypoint data in x-direction
        float rot_waypoint_x = 0; ///<optimal waypoint in vehicle frame, x-direction
        float rot_waypoint_y = 0; ///<optimal waypoint in vehicle frame, ydirection

        visualization_msgs::Marker marker; //visualization stuff

        float look_ahead_distance; ///<Look-ahead parameter for choosing next optimal waypoint

        double steering_angle = 0.0; ///<optimal steering angle for the current iteration
        double steering_limit; ///<Actuation physical limit on the steering angle limit on the vehicle
        double high_velocity; ///<Upper limit of the vehicle velocity
        double low_velocity; ///<Lower limit of the vehicle velocity
        int last_index = -1; ///<Index of the selected optimal waypoint

        std::string waypoint_filename; ///<Address of the CSV file containing waypoints
        std::string pose_topic; ///< subscriber topic, particle filter publishes poses on this
        std::string drive_topic; ///< publisher topic, controller node publishes vehicle inputs on this
        std::string laser_topic; ///< subscriber topic, lidar publishes values on this
        std::string visualization_topic;

        mpcBlock::laserdata current_scan; ///< struct to hold the laser data

        //MPC stuff
        float lower_threshold; ///< The threshold that decides if there's an obstacle //TODO: Make this velocity based
        float midline_threshold; ///< Decides to ignore for large gaps that occurs in cornering
        float breakneck_steering; ///< Steering for when obstacle on waypoint //TODO: Tune
        float min_halfspace_width; ///< If halfspace on any side is less than this, set to zero. Prevents running into obstacle. //TODO: Tune
        float breakneck_steering_threshold; ///< Half-space width for breakneck_steering activation
        double Q_matrix_1;  ///<State cost on waypoint error (horizontal)
        double Q_matrix_2; ///< State cost on waypoint error (vertical)
        double R_matrix_1; ///< Input cosr
        double B_matrix; ///< velocity*deltaT (deltaT = 1/30)
    };

}