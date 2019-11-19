#pragma once

class run_cvxgenOptimization{
public:

    run_cvxgenOptimization(){
        set_defaults();
        setup_indexing();
        run_cvxgenOptimization::generate_matrices();
    }

    static void generate_matrices(){

        params.A[0] = 1;
        params.A[1] = 0;
        params.A[2] = 0;
        params.A[3] = 1;

        params.B[0] = 7.0/30.0; // v*deltaT

        params.Q[0] = 4; //State cost
        params.Q[1] = 0;
        params.Q[2] = 0;
        params.Q[3] = 4;

        params.R[0] = 2; //Input cost
        params.R[1] = 0;
        params.R[2] = 0;
        params.R[3] = 0;

        params.Q_final[0] = 4; //Final state cost
        params.Q_final[0] = 0;
        params.Q_final[0] = 0;
        params.Q_final[0] = 4;

        // setting up initial position
        params.x_0[0] = 0;
        params.x_0[0] = 0;

        params.u_max[0] = 0.4189;
        params.u_max[1] = 1;

        params.s[0] = 0;

    }

    static void update_model(double y_waypoint, double x_waypoint, float y_mid){
        //goal point (waypoint for now)
        params.w[0] = y_waypoint;
        params.w[1] = x_waypoint;
        params.y_mid[0] = y_mid;
    }

    static float solve_mpc(double y_waypoint, double x_waypoint, float y_mid){
        run_cvxgenOptimization::update_model(y_waypoint, x_waypoint, y_mid);
        auto num_iters = solve();

        if(!work.converged){
            ROS_ERROR("Optimization not converged");
        }

        return vars.u_0[0];
    }

};
