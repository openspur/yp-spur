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

#ifndef __AUXLIB_ODOMETRY_SSM_H
#define __AUXLIB_ODOMETRY_SSM_H

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

int OdometrySSM_init(void);
int SSM_get_pos_GL_time(double time, double *x, double *y, double *theta);
int SSM_get_pos_BS_time(double time, double *x, double *y, double *theta);
double SSM_get_pos_GL(double *x, double *y, double *theta);
double SSM_get_pos_BS(double *x, double *y, double *theta);
double SSM_get_vel(double *v, double *w);
int SSM_near_pos_GL(double x, double y, double r);
int SSM_near_ang(double th, double d);
int SSM_over_line(double x, double y, double theta);

#ifdef __cplusplus
}
#endif  // __cplusplus
#endif  // __AUXLIB_ODOMETRY_SSM_H
