#pragma once

class run_cvxgenOptimization{
public:

    run_cvxgenOptimization(const double Q_matrix_1, const double Q_matrix_2, const double R_matrix_1, const double B_matrix){
        set_defaults();
        setup_indexing();
        run_cvxgenOptimization::generate_matrices(Q_matrix_1, Q_matrix_2, R_matrix_1, B_matrix);
    }

    void generate_matrices(const double Q_matrix_1, const double Q_matrix_2, const double R_matrix_1, const double B_matrix){

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

        params.u_max[0] = 0.4189;
        params.u_max[1] = 1;
    }

    void update_model(double y_waypoint, double x_waypoint){
        //goal point (waypoint for now)
        params.w[0] = y_waypoint;
        params.w[1] = x_waypoint;
    }

    float solve_mpc(double y_waypoint, double x_waypoint){
        run_cvxgenOptimization::update_model(y_waypoint, x_waypoint);
        auto num_iters = solve();

        if(!work.converged){
            ROS_ERROR("Optimization not converged");
        }

        return vars.u_0[0];
    }

};
