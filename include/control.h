// Copyright (c) 2010-2016 The YP-Spur Authors, except where otherwise indicated.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef CONTROL_H
#define CONTROL_H

#include <command.h>
#include <odometry.h>

int robot_speed_smooth(SpurUserParamsPtr spur);
void robot_speed(SpurUserParamsPtr spur);
int motor_control(SpurUserParamsPtr spur);
void apply_motor_torque(SpurUserParamsPtr spur);
void apply_motor_speed(SpurUserParamsPtr spur);

void update_ref_speed(SpurUserParamsPtr spur);

double timeoptimal_servo(double err, double vel_max, double vel, double acc);
double timeoptimal_servo2(double err, double vel_max, double vel, double acc, double vel_end);

double dist_pos(OdometryPtr odm, SpurUserParamsPtr spur);
double trans_q(double theta);

double circle_follow(OdometryPtr odm, SpurUserParamsPtr spur);
double line_follow(OdometryPtr odm, SpurUserParamsPtr spur);
int stop_line(OdometryPtr odm, SpurUserParamsPtr spur);
double spin(OdometryPtr odm, SpurUserParamsPtr spur);
double orient(OdometryPtr odm, SpurUserParamsPtr spur);
void wheel_vel(OdometryPtr odm, SpurUserParamsPtr spur);
void wheel_angle(OdometryPtr odm, SpurUserParamsPtr spur);
void wheel_torque(OdometryPtr odm, SpurUserParamsPtr spur, double *torque);

double regurator(double d, double q, double r, double v_max, double w_max, SpurUserParamsPtr spur);

double gravity_compensation(OdometryPtr odm, SpurUserParamsPtr spur);
void run_control(Odometry odometry, SpurUserParamsPtr spur);
void control_loop(void);

void init_control_thread(pthread_t *thread);

void set_run_mode(SpurRunMode mode);
SpurRunMode get_run_mode(void);

#endif  // CONTROL_H
