% csolve  Solves a custom quadratic program very rapidly.
%
% [vars, status] = csolve(params, settings)
%
% solves the convex optimization problem
%
%   minimize(quad_form(x_0 - w, Q) + quad_form(u_0, R) + quad_form(x_1 - w, Q) + quad_form(u_1, R) + quad_form(x_2 - w, Q) + quad_form(u_2, R) + quad_form(x_3 - w, Q) + quad_form(u_3, R) + quad_form(x_4 - w, Q) + quad_form(u_4, R) + quad_form(x_5 - w, Q) + quad_form(u_5, R) + quad_form(x_6 - w, Q) + quad_form(u_6, R) + quad_form(x_7 - w, Q) + quad_form(u_7, R) + quad_form(x_8 - w, Q) + quad_form(u_8, R) + quad_form(x_9 - w, Q) + quad_form(u_9, R) + quad_form(x_10 - w, Q) + quad_form(u_10, R) + quad_form(x_11 - w, Q) + quad_form(u_11, R) + quad_form(x_12 - w, Q) + quad_form(u_12, R) + quad_form(x_13 - w, Q) + quad_form(u_13, R) + quad_form(x_14 - w, Q) + quad_form(u_14, R) + quad_form(x_15 - w, Q) + quad_form(u_15, R) + quad_form(x_16 - w, Q) + quad_form(u_16, R) + quad_form(x_17 - w, Q) + quad_form(u_17, R) + quad_form(x_18 - w, Q) + quad_form(u_18, R) + quad_form(x_19 - w, Q) + quad_form(u_19, R) + quad_form(x_20 - w, Q) + quad_form(u_20, R) + quad_form(x_21 - w, Q) + quad_form(u_21, R) + quad_form(x_22 - w, Q) + quad_form(u_22, R) + quad_form(x_23 - w, Q) + quad_form(u_23, R) + quad_form(x_24 - w, Q) + quad_form(u_24, R) + quad_form(x_25 - w, Q) + quad_form(u_25, R) + quad_form(x_26 - w, Q) + quad_form(u_26, R) + quad_form(x_27 - w, Q) + quad_form(u_27, R) + quad_form(x_28 - w, Q) + quad_form(u_28, R) + quad_form(x_29 - w, Q) + quad_form(u_29, R) + quad_form(x_30 - w, Q) + quad_form(u_30, R) + quad_form(x_31 - w, Q) + quad_form(u_31, R) + quad_form(x_32 - w, Q) + quad_form(u_32, R) + quad_form(x_33 - w, Q) + quad_form(u_33, R) + quad_form(x_34 - w, Q) + quad_form(u_34, R) + quad_form(x_35 - w, Q) + quad_form(u_35, R) + quad_form(x_36 - w, Q) + quad_form(u_36, R) + quad_form(x_37 - w, Q) + quad_form(u_37, R) + quad_form(x_38 - w, Q) + quad_form(u_38, R) + quad_form(x_39 - w, Q) + quad_form(u_39, R) + quad_form(x_40 - w, Q) + quad_form(u_40, R) + quad_form(x_41 - w, Q_final))
%   subject to
%     x_1 == A*x_0 + B*u_0
%     x_2 == A*x_1 + B*u_1
%     x_3 == A*x_2 + B*u_2
%     x_4 == A*x_3 + B*u_3
%     x_5 == A*x_4 + B*u_4
%     x_6 == A*x_5 + B*u_5
%     x_7 == A*x_6 + B*u_6
%     x_8 == A*x_7 + B*u_7
%     x_9 == A*x_8 + B*u_8
%     x_10 == A*x_9 + B*u_9
%     x_11 == A*x_10 + B*u_10
%     x_12 == A*x_11 + B*u_11
%     x_13 == A*x_12 + B*u_12
%     x_14 == A*x_13 + B*u_13
%     x_15 == A*x_14 + B*u_14
%     x_16 == A*x_15 + B*u_15
%     x_17 == A*x_16 + B*u_16
%     x_18 == A*x_17 + B*u_17
%     x_19 == A*x_18 + B*u_18
%     x_20 == A*x_19 + B*u_19
%     x_21 == A*x_20 + B*u_20
%     x_22 == A*x_21 + B*u_21
%     x_23 == A*x_22 + B*u_22
%     x_24 == A*x_23 + B*u_23
%     x_25 == A*x_24 + B*u_24
%     x_26 == A*x_25 + B*u_25
%     x_27 == A*x_26 + B*u_26
%     x_28 == A*x_27 + B*u_27
%     x_29 == A*x_28 + B*u_28
%     x_30 == A*x_29 + B*u_29
%     x_31 == A*x_30 + B*u_30
%     x_32 == A*x_31 + B*u_31
%     x_33 == A*x_32 + B*u_32
%     x_34 == A*x_33 + B*u_33
%     x_35 == A*x_34 + B*u_34
%     x_36 == A*x_35 + B*u_35
%     x_37 == A*x_36 + B*u_36
%     x_38 == A*x_37 + B*u_37
%     x_39 == A*x_38 + B*u_38
%     x_40 == A*x_39 + B*u_39
%     x_41 == A*x_40 + B*u_40
%     abs(u_0) <= u_max
%     abs(u_1) <= u_max
%     abs(u_2) <= u_max
%     abs(u_3) <= u_max
%     abs(u_4) <= u_max
%     abs(u_5) <= u_max
%     abs(u_6) <= u_max
%     abs(u_7) <= u_max
%     abs(u_8) <= u_max
%     abs(u_9) <= u_max
%     abs(u_10) <= u_max
%     abs(u_11) <= u_max
%     abs(u_12) <= u_max
%     abs(u_13) <= u_max
%     abs(u_14) <= u_max
%     abs(u_15) <= u_max
%     abs(u_16) <= u_max
%     abs(u_17) <= u_max
%     abs(u_18) <= u_max
%     abs(u_19) <= u_max
%     abs(u_20) <= u_max
%     abs(u_21) <= u_max
%     abs(u_22) <= u_max
%     abs(u_23) <= u_max
%     abs(u_24) <= u_max
%     abs(u_25) <= u_max
%     abs(u_26) <= u_max
%     abs(u_27) <= u_max
%     abs(u_28) <= u_max
%     abs(u_29) <= u_max
%     abs(u_30) <= u_max
%     abs(u_31) <= u_max
%     abs(u_32) <= u_max
%     abs(u_33) <= u_max
%     abs(u_34) <= u_max
%     abs(u_35) <= u_max
%     abs(u_36) <= u_max
%     abs(u_37) <= u_max
%     abs(u_38) <= u_max
%     abs(u_39) <= u_max
%     abs(u_40) <= u_max
%     norm(u_1(1) - u_0(1), inf) <= S
%     norm(u_2(1) - u_1(1), inf) <= S
%     norm(u_3(1) - u_2(1), inf) <= S
%     norm(u_4(1) - u_3(1), inf) <= S
%     norm(u_5(1) - u_4(1), inf) <= S
%     norm(u_6(1) - u_5(1), inf) <= S
%     norm(u_7(1) - u_6(1), inf) <= S
%     norm(u_8(1) - u_7(1), inf) <= S
%     norm(u_9(1) - u_8(1), inf) <= S
%     norm(u_10(1) - u_9(1), inf) <= S
%     norm(u_11(1) - u_10(1), inf) <= S
%     norm(u_12(1) - u_11(1), inf) <= S
%     norm(u_13(1) - u_12(1), inf) <= S
%     norm(u_14(1) - u_13(1), inf) <= S
%     norm(u_15(1) - u_14(1), inf) <= S
%     norm(u_16(1) - u_15(1), inf) <= S
%     norm(u_17(1) - u_16(1), inf) <= S
%     norm(u_18(1) - u_17(1), inf) <= S
%     norm(u_19(1) - u_18(1), inf) <= S
%     norm(u_20(1) - u_19(1), inf) <= S
%     norm(u_21(1) - u_20(1), inf) <= S
%     norm(u_22(1) - u_21(1), inf) <= S
%     norm(u_23(1) - u_22(1), inf) <= S
%     norm(u_24(1) - u_23(1), inf) <= S
%     norm(u_25(1) - u_24(1), inf) <= S
%     norm(u_26(1) - u_25(1), inf) <= S
%     norm(u_27(1) - u_26(1), inf) <= S
%     norm(u_28(1) - u_27(1), inf) <= S
%     norm(u_29(1) - u_28(1), inf) <= S
%     norm(u_30(1) - u_29(1), inf) <= S
%     norm(u_31(1) - u_30(1), inf) <= S
%     norm(u_32(1) - u_31(1), inf) <= S
%     norm(u_33(1) - u_32(1), inf) <= S
%     norm(u_34(1) - u_33(1), inf) <= S
%     norm(u_35(1) - u_34(1), inf) <= S
%     norm(u_36(1) - u_35(1), inf) <= S
%     norm(u_37(1) - u_36(1), inf) <= S
%     norm(u_38(1) - u_37(1), inf) <= S
%     norm(u_39(1) - u_38(1), inf) <= S
%     norm(u_40(1) - u_39(1), inf) <= S
%     x_1 >= x_lower
%     x_2 >= x_lower
%     x_3 >= x_lower
%     x_4 >= x_lower
%     x_5 >= x_lower
%     x_6 >= x_lower
%     x_7 >= x_lower
%     x_8 >= x_lower
%     x_9 >= x_lower
%     x_10 >= x_lower
%     x_11 >= x_lower
%     x_12 >= x_lower
%     x_13 >= x_lower
%     x_14 >= x_lower
%     x_15 >= x_lower
%     x_16 >= x_lower
%     x_17 >= x_lower
%     x_18 >= x_lower
%     x_19 >= x_lower
%     x_20 >= x_lower
%     x_21 >= x_lower
%     x_22 >= x_lower
%     x_23 >= x_lower
%     x_24 >= x_lower
%     x_25 >= x_lower
%     x_26 >= x_lower
%     x_27 >= x_lower
%     x_28 >= x_lower
%     x_29 >= x_lower
%     x_30 >= x_lower
%     x_31 >= x_lower
%     x_32 >= x_lower
%     x_33 >= x_lower
%     x_34 >= x_lower
%     x_35 >= x_lower
%     x_36 >= x_lower
%     x_37 >= x_lower
%     x_38 >= x_lower
%     x_39 >= x_lower
%     x_40 >= x_lower
%     x_1 <= x_upper
%     x_2 <= x_upper
%     x_3 <= x_upper
%     x_4 <= x_upper
%     x_5 <= x_upper
%     x_6 <= x_upper
%     x_7 <= x_upper
%     x_8 <= x_upper
%     x_9 <= x_upper
%     x_10 <= x_upper
%     x_11 <= x_upper
%     x_12 <= x_upper
%     x_13 <= x_upper
%     x_14 <= x_upper
%     x_15 <= x_upper
%     x_16 <= x_upper
%     x_17 <= x_upper
%     x_18 <= x_upper
%     x_19 <= x_upper
%     x_20 <= x_upper
%     x_21 <= x_upper
%     x_22 <= x_upper
%     x_23 <= x_upper
%     x_24 <= x_upper
%     x_25 <= x_upper
%     x_26 <= x_upper
%     x_27 <= x_upper
%     x_28 <= x_upper
%     x_29 <= x_upper
%     x_30 <= x_upper
%     x_31 <= x_upper
%     x_32 <= x_upper
%     x_33 <= x_upper
%     x_34 <= x_upper
%     x_35 <= x_upper
%     x_36 <= x_upper
%     x_37 <= x_upper
%     x_38 <= x_upper
%     x_39 <= x_upper
%     x_40 <= x_upper
%
% with variables
%      u_0   2 x 1
%      u_1   2 x 1
%      u_2   2 x 1
%      u_3   2 x 1
%      u_4   2 x 1
%      u_5   2 x 1
%      u_6   2 x 1
%      u_7   2 x 1
%      u_8   2 x 1
%      u_9   2 x 1
%     u_10   2 x 1
%     u_11   2 x 1
%     u_12   2 x 1
%     u_13   2 x 1
%     u_14   2 x 1
%     u_15   2 x 1
%     u_16   2 x 1
%     u_17   2 x 1
%     u_18   2 x 1
%     u_19   2 x 1
%     u_20   2 x 1
%     u_21   2 x 1
%     u_22   2 x 1
%     u_23   2 x 1
%     u_24   2 x 1
%     u_25   2 x 1
%     u_26   2 x 1
%     u_27   2 x 1
%     u_28   2 x 1
%     u_29   2 x 1
%     u_30   2 x 1
%     u_31   2 x 1
%     u_32   2 x 1
%     u_33   2 x 1
%     u_34   2 x 1
%     u_35   2 x 1
%     u_36   2 x 1
%     u_37   2 x 1
%     u_38   2 x 1
%     u_39   2 x 1
%     u_40   2 x 1
%      x_1   2 x 1
%      x_2   2 x 1
%      x_3   2 x 1
%      x_4   2 x 1
%      x_5   2 x 1
%      x_6   2 x 1
%      x_7   2 x 1
%      x_8   2 x 1
%      x_9   2 x 1
%     x_10   2 x 1
%     x_11   2 x 1
%     x_12   2 x 1
%     x_13   2 x 1
%     x_14   2 x 1
%     x_15   2 x 1
%     x_16   2 x 1
%     x_17   2 x 1
%     x_18   2 x 1
%     x_19   2 x 1
%     x_20   2 x 1
%     x_21   2 x 1
%     x_22   2 x 1
%     x_23   2 x 1
%     x_24   2 x 1
%     x_25   2 x 1
%     x_26   2 x 1
%     x_27   2 x 1
%     x_28   2 x 1
%     x_29   2 x 1
%     x_30   2 x 1
%     x_31   2 x 1
%     x_32   2 x 1
%     x_33   2 x 1
%     x_34   2 x 1
%     x_35   2 x 1
%     x_36   2 x 1
%     x_37   2 x 1
%     x_38   2 x 1
%     x_39   2 x 1
%     x_40   2 x 1
%     x_41   2 x 1
%
% and parameters
%        A   2 x 2
%        B   1 x 1
%        Q   2 x 2    PSD
%  Q_final   2 x 2    PSD
%        R   2 x 2    PSD
%        S   1 x 1    positive
%    u_max   2 x 1    positive
%        w   2 x 1
%      x_0   2 x 1
%  x_lower   2 x 1
%  x_upper   2 x 1
%
% Note:
%   - Check status.converged, which will be 1 if optimization succeeded.
%   - You don't have to specify settings if you don't want to.
%   - To hide output, use settings.verbose = 0.
%   - To change iterations, use settings.max_iters = 20.
%   - You may wish to compare with cvxsolve to check the solver is correct.
%
% Specify params.A, ..., params.x_upper, then run
%   [vars, status] = csolve(params, settings)
% Produced by CVXGEN, 2019-12-04 21:43:18 -0500.
% CVXGEN is Copyright (C) 2006-2017 Jacob Mattingley, jem@cvxgen.com.
% The code in this file is Copyright (C) 2006-2017 Jacob Mattingley.
% CVXGEN, or solvers produced by CVXGEN, cannot be used for commercial
% applications without prior written permission from Jacob Mattingley.

% Filename: csolve.m.
% Description: Help file for the Matlab solver interface.
