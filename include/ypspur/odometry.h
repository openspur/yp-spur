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

#ifndef YPSPUR_ODOMETRY_H
#define YPSPUR_ODOMETRY_H

#include <ypspur/shvel-param.h>

#include <carte2d/cartesian2d.h>
#include <ypspur.h>
#include <ypspur/odometry_type.h>
#include <ypspur/ypparam.h>

double time_estimate(int readnum);
void cstrans_odometry(YPSpur_cs cs, OdometryPtr dst_odm);
void cstrans_xy(YPSpur_cs src, YPSpur_cs dest, double* x, double* y, double* theta);
void odometry(OdometryPtr xp, short* cnt, short* pwm, double dt, double time);
int process_int4(
    OdometryPtr xp, ErrorStatePtr err, int param_id, int id, int value, double receive_time);
void odm_logging(OdometryPtr, double, double);
int odm_read(OdometryPtr odm, double* v, double* w);
void cs_odometry(YPSpur_cs cs, OdometryPtr dst_odm);
void init_odometry(void);
void init_coordinate_systems(void);
int odometry_receive_loop(void);

OdometryPtr get_odometry_ptr();
ErrorStatePtr get_error_state_ptr();
CSptr get_cs_pointer(YPSpur_cs cs);
void set_cs(YPSpur_cs cs, double x, double y, double theta);
void set_odometry_hook(OdometryHook fn);
OdometryHook get_odometry_hook();

#endif  // YPSPUR_ODOMETRY_H
