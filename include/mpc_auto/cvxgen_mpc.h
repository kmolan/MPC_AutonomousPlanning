#pragma once

#include<cvxgen/solver.h>

class run_cvxgenOptimization{
public:

    run_cvxgenOptimization(){
        set_defaults();
        setup.indexing();
    }

    void generate_matrices(double y_waypoint, double x_waypoint){
        void load_data(Params params) {
            params.A[0] = 1;
            params.A[1] = 0;
            params.A[2] = 0;
            params.A[3] = 1;

            params.B[0] = 2 * 0.1 // v*deltaT

            params.Q[0] = 1 //State cost is unity
            params.Q[1] = 0;
            params.Q[2] = 0;
            params.Q[3] = 1;

            params.R[0] = 0; //Input cost is zero
            params.R[1] = 0;
            params.R[2] = 0;
            params.R[3] = 0;

            params.Q_final[0] = 1; //Final state cost is unity
            params.Q_final[0] = 0;
            params.Q_final[0] = 0;
            params.Q_final[0] = 1;

            // setting up initial position
            x_0[0] = 0;
            x_0[0] = 0;

            //goal point (waypoint for now)
            w[0] = y_waypoint;
            w[1] = x_waypoint;

            u_max[0] = 0.4189;
            u_max[1] = 1;
        }

    }

    float solve_mpc(double y_waypoint, double x_waypoint){
        run_cvxgenOptimization::generate_matrices(double y_waypoint, double x_waypoint);
        num_iters = solve();

        return u_0[0];
    }

};
