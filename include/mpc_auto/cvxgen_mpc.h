/**Helper class to set and initiate the MPC optimization routine**/

#pragma once

#include <cvxgen/solver.h>
#include <ros/ros.h>

class run_cvxgenOptimization{
public:

    /*!
     * @brief default constructor
     * @param Q_matrix_1 State cost on y-direction
     * @param Q_matrix_2 State cost on x-direction
     * @param R_matrix_1 Input cost on steering angle
     * @param B_matrix Weight matrix for input
     */
    run_cvxgenOptimization(const double Q_matrix_1, const double Q_matrix_2, const double R_matrix_1, const double B_matrix){
        set_defaults();
        setup_indexing();
        run_cvxgenOptimization::generate_matrices(Q_matrix_1, Q_matrix_2, R_matrix_1, B_matrix);
    }

    /*!
     * @brief sets relevant parameters for MPC routine to handle
     * @param Q_matrix_1 State cost on y-direction
     * @param Q_matrix_2 State cost on x-direction
     * @param R_matrix_1 Input cost on steering angle
     * @param B_matrix Weight matrix for input
     */
    static void generate_matrices(const double Q_matrix_1, const double Q_matrix_2, const double R_matrix_1, const double B_matrix){

        params.A[0] = 1;
        params.A[1] = 0;
        params.A[2] = 0;
        params.A[3] = 1;

        params.B[0] = B_matrix; // v*deltaT

        params.Q[0] = Q_matrix_1; //State cost
        params.Q[1] = 0;
        params.Q[2] = 0;
        params.Q[3] = Q_matrix_2;

        params.R[0] = R_matrix_1; //Input cost
        params.R[1] = 0;
        params.R[2] = 0;
        params.R[3] = 0;

        params.Q_final[0] = Q_matrix_1; //Final state cost
        params.Q_final[0] = 0;
        params.Q_final[0] = 0;
        params.Q_final[0] = Q_matrix_2;

        // setting up initial position
        params.x_0[0] = 0;
        params.x_0[0] = 0;

        //Input constraints
        params.u_max[0] = 0.4189;
        params.u_max[1] = 1;

        params.S[0] = 0.1; //Slew Rate
    }

    /*!
     * @brief updates MPC model with new optimal waypoints
     * @param y_waypoint the current waypoint y-coordinate
     * @param x_waypoint the current waypoint x-coordinate
     * @param y_lower_dist the halfspace constraint in negative direction
     * @param y_upper_dist the halfspace constraint in positive direction
     */
    static void update_model(double y_waypoint, double x_waypoint, double y_lower_dist, double y_upper_dist){
        params.w[0] = y_waypoint;
        params.w[1] = x_waypoint;
        params.x_lower[0] = y_lower_dist + 0.1;
        params.x_lower[1] = -0.1;
        params.x_upper[0] = y_upper_dist - 0.1;
        params.x_upper[1] = x_waypoint + 0.3;
    }

    /*!
     * @brief calls the mpc pipeline to solve for optimal input
     * @param y_waypoint the current waypoint y-coordinate
     * @param x_waypoint the current waypoint x-coordinate
     * @param y_lower the halfspace constraint in negative direction
     * @param y_upper the halfspace constraint in positive direction
     * @return optimal steering angle
     */
    static float solve_mpc(double y_waypoint, double x_waypoint, double y_lower, double y_upper){
        run_cvxgenOptimization::update_model(y_waypoint, x_waypoint, y_lower, y_upper);

        settings.verbose = 0;
        settings.eps = 1e-2;

        auto num_iters = solve();

        if(!work.converged){
            ROS_ERROR("Optimization not converged");
        }

        return vars.u_0[0];
    }
};
