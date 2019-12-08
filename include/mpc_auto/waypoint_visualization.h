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
         * @brief subscribes to the current waypoint index
         * @param msg pointer to the current waypoint index
         */
        void waypoint_index_callback(const std_msgs::Float64::ConstPtr &msg);

        ros::NodeHandle nh; ///<NodeHandle of the node
        ros::Subscriber waypoint_index_subs; ///<Subscribes to the current waypoint index
        ros::Publisher vis_pub; ///< For visualization stuff on RViz

        std::vector <std::vector<float>> waypoint_data_full; ///< Original raw waypoint data

        visualization_msgs::Marker marker; ///<visualization marker object
        std::string visualization_topic; ///<topic over which vis_pub publishes
        std::string waypoint_index_topic; ///<Topic over which marker_x_subs subscribes
        std::string waypoint_filename; ///<Address of the CSV file containing waypoints

        double marker_x; ///<Current x-waypoint
        double marker_y; ///<Current y-waypoint

    };
}