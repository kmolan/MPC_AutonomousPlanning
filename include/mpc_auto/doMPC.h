#pragma once

#include <ros/ros.h>
#include <string>
#include <std_msgs/Float64.h>
#include <nav_msgs/Odometry.h>
#include <cmath>
#include <sensor_msgs/LaserScan.h>
#include <ackermann_msgs/AckermannDriveStamped.h>

#include "cvxgen/solver.h"

#include "cvxgen_mpc.h"

namespace mpcBlock{

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

        double angle_increment; ///< the angle between two consecutive readings
        double angle_sweep; ///<the total sweep of angle for lidar readings

        //TODO:Refactor
        int zero_angle; ///<the index for zero angle (straight ahead) in lidar readings
        int left_angle; ///<the index for +90 degrees in lidar readings (left of the vehicle)
        int right_angle; ///<the index for -90 degrees in lidar readings (right of the vehicle)

        float y_upper_distance; ///<the y-axis distance from the vehicle towards closest obstacle at left side of the car
        float y_lower_distance; ///<the y-axis distance from the vehicle towards closest obstacle at right side of the car
        float y_mid_distance; ///<the mean of upper and lower distances
    };

    class doMPC{

    public:

        /*!
         * @brief default constructor
         */
        doMPC();

        /*!
         * @brief calls all the class publishers
         */
        void publisherCallback();

        /*!
         * @brief print stuff on console for debugging
         */
        void debug();

        /*!
         * @brief Calls the MPC obtimization routine to solve for optimal input
         */
        void controller_callback();

    private:

        /*!
         * @brief fetches parameters and assigns it to member variables
         */
        void getParams();

        /*!
         * @brief subscribes to the lidar readings for obstacle avoidance
         * @param scan_msg container pointer for the lidar readings
         */
        void lidar_callback(const sensor_msgs::LaserScan::ConstPtr &scan_msg);

        /*!
         * @brief subscribes to the x_waypoint
         * @param msg pointer to the current x-waypoint
         */
        void marker_x_callback(const std_msgs::Float64::ConstPtr &msg);

        /*!
         * @brief subscribes to the y_waypoint
         * @param msg pointer to the current y-waypoint
         */
        void marker_y_callback(const std_msgs::Float64::ConstPtr &msg);

        /*!
         * @brief subscribes to the final steering angle at waypoint
         * @param msg pointer to final steering angle
         */
        void final_theta_callback(const std_msgs::Float64::ConstPtr &msg);

        ros::NodeHandle nodeH; ///<NodeHandle of the node

        ros::Subscriber marker_x_subs;
        ros::Subscriber marker_y_subs;
        ros::Subscriber lidar_sub; ///< subscribes to the lidar scan for obstacle avoidance
        ros::Subscriber final_theta_subs;

        ros::Publisher drive_pub; ///< publishes on vehicle inputs (steering and velocity)

        std::string drive_topic; ///< publisher topic, controller node publishes vehicle inputs on this
        std::string laser_topic; ///< subscriber topic, lidar publishes values on this
        std::string marker_x_topic; ///<Topic over which marker_x_pubs subscribes
        std::string marker_y_topic; ///<Topic over which marker_y_pubs subscribes
        std::string theta_topic; ///< subscriber topic, final steering angle at waypoint

        mpcBlock::laserdata current_scan; ///< struct to hold the laser data

        double steering_angle = 0.0; ///<optimal steering angle for the current iteration
        double prev_steering_angle = 0.0; ///< Previous optimal steering angle
        double steering_angle_change; // TODO:add
        double steering_limit; ///<Actuation physical limit on the steering angle limit on the vehicle
        double high_velocity; ///<Upper limit of the vehicle velocity
        double low_velocity; ///<Lower limit of the vehicle velocity

        double rot_waypoint_x = 0; ///<optimal waypoint in vehicle frame, x-direction
        double rot_waypoint_y = 0; ///<optimal waypoint in vehicle frame, y-direction
        double chosen_theta = 0;

        //MPC parameters
        float lower_threshold; ///< The threshold that decides if there's an obstacle //TODO: Make this velocity based
        float midline_threshold; ///< Decides to ignore for large gaps that occurs in cornering
        float breakneck_steering; ///< Steering for when obstacle on waypoint //TODO: Tune
        float min_halfspace_width; ///< If halfspace on any side is less than this, set to zero. Prevents running into obstacle. //TODO: Tune
        float breakneck_steering_threshold; ///< Half-space width for breakneck_steering activation
        double Q_matrix_1;  ///<State cost on waypoint error (horizontal)
        double Q_matrix_2; ///< State cost on waypoint error (vertical)
        double R_matrix_1; ///< Input cost
        double B_matrix; ///< velocity*deltaT (deltaT = 1/30)

    };
}