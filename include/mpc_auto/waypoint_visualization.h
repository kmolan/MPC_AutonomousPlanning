#pragma once

#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <string>
#include <std_msgs/Float64.h>

namespace mpcBlock{

    class visualizeWaypoint{
    public:

        /*!
         * @brief default constructor
         */
        visualizeWaypoint();

        /*!
         * @brief calls the class publishers
         */
        void publisherCallback();

        /*!
         * @brief print stuff on console for debugging
         */
        void debug();

    private:

        /*!
         * @brief fetches parameters and assigns it to member variables
         */
        void getParams();

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

        ros::NodeHandle nh; ///<NodeHandle of the node
        ros::Subscriber marker_x_subs; ///<Subscribes to the x-coordinate of current waypoint
        ros::Subscriber marker_y_subs; ///<Subscribes to the y-coordinate to current waypoint
        ros::Publisher vis_pub; ///< For visualization stuff on RViz

        visualization_msgs::Marker marker; ///<visualization marker object
        std::string visualization_topic; ///<topic over which vis_pub publishes
        std::string marker_x_topic; ///<Topic over which marker_x_subs subscribes
        std::string marker_y_topic; ///<Topic over which marker_y_subs subscribes

        std_msgs::Float64 marker_x; ///<Container for current waypoint data, x-coordinate
        std_msgs::Float64 marker_y; ///<Container for current waypoint data, y-coordinate
    };
}