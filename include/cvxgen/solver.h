/* Produced by CVXGEN, 2019-12-04 21:43:50 -0500.  */
/* CVXGEN is Copyright (C) 2006-2017 Jacob Mattingley, jem@cvxgen.com. */
/* The code in this file is Copyright (C) 2006-2017 Jacob Mattingley. */
/* CVXGEN, or solvers produced by CVXGEN, cannot be used for commercial */
/* applications without prior written permission from Jacob Mattingley. */

/* Filename: solver.h. */
/* Description: Header file with relevant definitions. */
#ifndef SOLVER_H
#define SOLVER_H
/* Uncomment the next line to remove all library dependencies. */
/*#define ZERO_LIBRARY_MODE */
#ifdef MATLAB_MEX_FILE
/* Matlab functions. MATLAB_MEX_FILE will be defined by the mex compiler. */
/* If you are not using the mex compiler, this functionality will not intrude, */
/* as it will be completely disabled at compile-time. */
#include "mex.h"
#else
#ifndef ZERO_LIBRARY_MODE
#include <stdio.h>
#endif
#endif
/* Space must be allocated somewhere (testsolver.c, csolve.c or your own */
/* program) for the global variables vars, params, work and settings. */
/* At the bottom of this file, they are externed. */
#ifndef ZERO_LIBRARY_MODE
#include <math.h>
#define pm(A, m, n) printmatrix(#A, A, m, n, 1)
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Params_t {
  double x_0[2];
  double w[2];
  double Q[4];
  double R[4];
  double Q_final[4];
  double A[4];
  double B[1];
  double u_max[2];
  double S[1];
  double x_lower[2];
  double x_upper[2];
  double *x[1];
} Params;
typedef struct Vars_t {
  double *u_0; /* 2 rows. */
  double *x_1; /* 2 rows. */
  double *u_1; /* 2 rows. */
  double *x_2; /* 2 rows. */
  double *u_2; /* 2 rows. */
  double *x_3; /* 2 rows. */
  double *u_3; /* 2 rows. */
  double *x_4; /* 2 rows. */
  double *u_4; /* 2 rows. */
  double *x_5; /* 2 rows. */
  double *u_5; /* 2 rows. */
  double *x_6; /* 2 rows. */
  double *u_6; /* 2 rows. */
  double *x_7; /* 2 rows. */
  double *u_7; /* 2 rows. */
  double *x_8; /* 2 rows. */
  double *u_8; /* 2 rows. */
  double *x_9; /* 2 rows. */
  double *u_9; /* 2 rows. */
  double *x_10; /* 2 rows. */
  double *u_10; /* 2 rows. */
  double *x_11; /* 2 rows. */
  double *u_11; /* 2 rows. */
  double *x_12; /* 2 rows. */
  double *u_12; /* 2 rows. */
  double *x_13; /* 2 rows. */
  double *u_13; /* 2 rows. */
  double *x_14; /* 2 rows. */
  double *u_14; /* 2 rows. */
  double *x_15; /* 2 rows. */
  double *u_15; /* 2 rows. */
  double *x_16; /* 2 rows. */
  double *u_16; /* 2 rows. */
  double *x_17; /* 2 rows. */
  double *u_17; /* 2 rows. */
  double *x_18; /* 2 rows. */
  double *u_18; /* 2 rows. */
  double *x_19; /* 2 rows. */
  double *u_19; /* 2 rows. */
  double *x_20; /* 2 rows. */
  double *u_20; /* 2 rows. */
  double *x_21; /* 2 rows. */
  double *u_21; /* 2 rows. */
  double *x_22; /* 2 rows. */
  double *u_22; /* 2 rows. */
  double *x_23; /* 2 rows. */
  double *u_23; /* 2 rows. */
  double *x_24; /* 2 rows. */
  double *u_24; /* 2 rows. */
  double *x_25; /* 2 rows. */
  double *u_25; /* 2 rows. */
  double *x_26; /* 2 rows. */
  double *u_26; /* 2 rows. */
  double *x_27; /* 2 rows. */
  double *u_27; /* 2 rows. */
  double *x_28; /* 2 rows. */
  double *u_28; /* 2 rows. */
  double *x_29; /* 2 rows. */
  double *u_29; /* 2 rows. */
  double *x_30; /* 2 rows. */
  double *u_30; /* 2 rows. */
  double *x_31; /* 2 rows. */
  double *u_31; /* 2 rows. */
  double *x_32; /* 2 rows. */
  double *u_32; /* 2 rows. */
  double *x_33; /* 2 rows. */
  double *u_33; /* 2 rows. */
  double *x_34; /* 2 rows. */
  double *u_34; /* 2 rows. */
  double *x_35; /* 2 rows. */
  double *u_35; /* 2 rows. */
  double *x_36; /* 2 rows. */
  double *u_36; /* 2 rows. */
  double *x_37; /* 2 rows. */
  double *u_37; /* 2 rows. */
  double *x_38; /* 2 rows. */
  double *u_38; /* 2 rows. */
  double *x_39; /* 2 rows. */
  double *u_39; /* 2 rows. */
  double *x_40; /* 2 rows. */
  double *u_40; /* 2 rows. */
  double *x_41; /* 2 rows. */
  double *t_01; /* 2 rows. */
  double *t_02; /* 2 rows. */
  double *t_03; /* 2 rows. */
  double *t_04; /* 2 rows. */
  double *t_05; /* 2 rows. */
  double *t_06; /* 2 rows. */
  double *t_07; /* 2 rows. */
  double *t_08; /* 2 rows. */
  double *t_09; /* 2 rows. */
  double *t_10; /* 2 rows. */
  double *t_11; /* 2 rows. */
  double *t_12; /* 2 rows. */
  double *t_13; /* 2 rows. */
  double *t_14; /* 2 rows. */
  double *t_15; /* 2 rows. */
  double *t_16; /* 2 rows. */
  double *t_17; /* 2 rows. */
  double *t_18; /* 2 rows. */
  double *t_19; /* 2 rows. */
  double *t_20; /* 2 rows. */
  double *t_21; /* 2 rows. */
  double *t_22; /* 2 rows. */
  double *t_23; /* 2 rows. */
  double *t_24; /* 2 rows. */
  double *t_25; /* 2 rows. */
  double *t_26; /* 2 rows. */
  double *t_27; /* 2 rows. */
  double *t_28; /* 2 rows. */
  double *t_29; /* 2 rows. */
  double *t_30; /* 2 rows. */
  double *t_31; /* 2 rows. */
  double *t_32; /* 2 rows. */
  double *t_33; /* 2 rows. */
  double *t_34; /* 2 rows. */
  double *t_35; /* 2 rows. */
  double *t_36; /* 2 rows. */
  double *t_37; /* 2 rows. */
  double *t_38; /* 2 rows. */
  double *t_39; /* 2 rows. */
  double *t_40; /* 2 rows. */
  double *t_41; /* 2 rows. */
  double *t_42; /* 1 rows. */
  double *t_43; /* 1 rows. */
  double *t_44; /* 1 rows. */
  double *t_45; /* 1 rows. */
  double *t_46; /* 1 rows. */
  double *t_47; /* 1 rows. */
  double *t_48; /* 1 rows. */
  double *t_49; /* 1 rows. */
  double *t_50; /* 1 rows. */
  double *t_51; /* 1 rows. */
  double *t_52; /* 1 rows. */
  double *t_53; /* 1 rows. */
  double *t_54; /* 1 rows. */
  double *t_55; /* 1 rows. */
  double *t_56; /* 1 rows. */
  double *t_57; /* 1 rows. */
  double *t_58; /* 1 rows. */
  double *t_59; /* 1 rows. */
  double *t_60; /* 1 rows. */
  double *t_61; /* 1 rows. */
  double *t_62; /* 1 rows. */
  double *t_63; /* 1 rows. */
  double *t_64; /* 1 rows. */
  double *t_65; /* 1 rows. */
  double *t_66; /* 1 rows. */
  double *t_67; /* 1 rows. */
  double *t_68; /* 1 rows. */
  double *t_69; /* 1 rows. */
  double *t_70; /* 1 rows. */
  double *t_71; /* 1 rows. */
  double *t_72; /* 1 rows. */
  double *t_73; /* 1 rows. */
  double *t_74; /* 1 rows. */
  double *t_75; /* 1 rows. */
  double *t_76; /* 1 rows. */
  double *t_77; /* 1 rows. */
  double *t_78; /* 1 rows. */
  double *t_79; /* 1 rows. */
  double *t_80; /* 1 rows. */
  double *t_81; /* 1 rows. */
  double *u[41];
  double *x[42];
} Vars;
typedef struct Workspace_t {
  double h[526];
  double s_inv[526];
  double s_inv_z[526];
  double b[82];
  double q[286];
  double rhs[1420];
  double x[1420];
  double *s;
  double *z;
  double *y;
  double lhs_aff[1420];
  double lhs_cc[1420];
  double buffer[1420];
  double buffer2[1420];
  double KKT[2998];
  double L[2225];
  double d[1420];
  double v[1420];
  double d_inv[1420];
  double gap;
  double optval;
  double ineq_resid_squared;
  double eq_resid_squared;
  double block_33[1];
  /* Pre-op symbols. */
  double quad_902543003648[1];
  double quad_622431752192[1];
  double quad_781620764672[1];
  int converged;
} Workspace;
typedef struct Settings_t {
  double resid_tol;
  double eps;
  int max_iters;
  int refine_steps;
  int better_start;
  /* Better start obviates the need for s_init and z_init. */
  double s_init;
  double z_init;
  int verbose;
  /* Show extra details of the iterative refinement steps. */
  int verbose_refinement;
  int debug;
  /* For regularization. Minimum value of abs(D_ii) in the kkt D factor. */
  double kkt_reg;
} Settings;
extern Vars vars;
extern Params params;
extern Workspace work;
extern Settings settings;
/* Function definitions in ldl.c: */
void ldl_solve(double *target, double *var);
void ldl_factor(void);
double check_factorization(void);
void matrix_multiply(double *result, double *source);
double check_residual(double *target, double *multiplicand);
void fill_KKT(void);

/* Function definitions in matrix_support.c: */
void multbymA(double *lhs, double *rhs);
void multbymAT(double *lhs, double *rhs);
void multbymG(double *lhs, double *rhs);
void multbymGT(double *lhs, double *rhs);
void multbyP(double *lhs, double *rhs);
void fillq(void);
void fillh(void);
void fillb(void);
void pre_ops(void);

/* Function definitions in solver.c: */
double eval_gap(void);
void set_defaults(void);
void setup_pointers(void);
void setup_indexed_params(void);
void setup_indexed_optvars(void);
void setup_indexing(void);
void set_start(void);
double eval_objv(void);
void fillrhs_aff(void);
void fillrhs_cc(void);
void refine(double *target, double *var);
double calc_ineq_resid_squared(void);
double calc_eq_resid_squared(void);
void better_start(void);
void fillrhs_start(void);
long solve(void);

/* Function definitions in testsolver.c: */
int main(int argc, char **argv);
void load_default_data(void);

/* Function definitions in util.c: */
void tic(void);
float toc(void);
float tocq(void);
void printmatrix(char *name, double *A, int m, int n, int sparse);
double unif(double lower, double upper);
float ran1(long*idum, int reset);
float randn_internal(long *idum, int reset);
double randn(void);
void reset_rand(void);

#ifdef __cplusplus
}
#endif

#endif
