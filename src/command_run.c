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

#include <math.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif  // HAVE_CONFIG_H

/* ボディパラメータ */
#include <shvel-param.h>

/* yp-spur用 */
#include <serial.h>
#include <param.h>
#include <control.h>
#include <command.h>

/* ライブラリ用 */
#include <ypspur.h>
#include <cartesian2d.h>

void line_com(int cs, double* data, SpurUserParamsPtr spur)
{
  double x, y, theta;

  x = data[0];
  y = data[1];
  theta = data[2];

  cstrans_xy(cs, CS_SP, &x, &y, &theta);

  spur->x = x;
  spur->y = y;
  spur->theta = theta;
  spur->run_mode = RUN_LINEFOLLOW;
}

void stop_line_com(int cs, double* data, SpurUserParamsPtr spur)
{
  double x, y, theta;

  x = data[0];
  y = data[1];
  theta = data[2];

  cstrans_xy(cs, CS_SP, &x, &y, &theta);

  spur->x = x;
  spur->y = y;
  spur->theta = theta;
  spur->run_mode = RUN_STOP_LINE;
}

void circle_com(int cs, double* data, SpurUserParamsPtr spur)
{
  double x, y, theta;

  x = data[0];
  y = data[1];
  theta = 0;

  cstrans_xy(cs, CS_SP, &x, &y, &theta);

  spur->x = x;
  spur->y = y;
  spur->radius = data[2];
  spur->run_mode = RUN_CIRCLEFOLLOW;
}

void spin_com(int cs, double* data, SpurUserParamsPtr spur)
{
  double x, y, theta;

  x = 0;
  y = 0;
  theta = data[0];

  cstrans_xy(cs, CS_SP, &x, &y, &theta);

  spur->theta = theta;
  spur->run_mode = RUN_SPIN;
}

void orient_com(int cs, double* data, SpurUserParamsPtr spur)
{
  double x, y, theta;

  x = 0;
  y = 0;
  theta = data[0];

  cstrans_xy(cs, CS_SP, &x, &y, &theta);

  spur->theta = theta;
  spur->run_mode = RUN_ORIENT;
}

void stop_com(double* data, SpurUserParamsPtr spur)
{
  spur->run_mode = RUN_STOP;
}

void free_com(double* data, SpurUserParamsPtr spur)
{
  spur->run_mode = RUN_FREE;
}

void openfree_com(double* data, SpurUserParamsPtr spur)
{
  spur->run_mode = RUN_OPENFREE;
}

void vel_com(double* data, SpurUserParamsPtr spur)
{
  spur->vref = data[0];
  spur->wref = data[1];
  spur->run_mode = RUN_VEL;
}

void wheel_vel_com(double* data, SpurUserParamsPtr spur)
{
  spur->wvelref[0] = data[0];
  spur->wvelref[1] = data[1];
  spur->run_mode = RUN_WHEEL_VEL;
}

void wheel_angle_com(double* data, SpurUserParamsPtr spur)
{
  double r, l;

  r = data[0];
  l = data[1];

  if (isset_p(YP_PARAM_MIN_WHEEL_ANGLE, MOTOR_RIGHT))
  {
    if (r < p(YP_PARAM_MIN_WHEEL_ANGLE, MOTOR_RIGHT))
      r = p(YP_PARAM_MIN_WHEEL_ANGLE, MOTOR_RIGHT);
  }
  if (isset_p(YP_PARAM_MIN_WHEEL_ANGLE, MOTOR_LEFT))
  {
    if (l < p(YP_PARAM_MIN_WHEEL_ANGLE, MOTOR_LEFT))
      l = p(YP_PARAM_MIN_WHEEL_ANGLE, MOTOR_LEFT);
  }
  if (isset_p(YP_PARAM_MAX_WHEEL_ANGLE, MOTOR_RIGHT))
  {
    if (r > p(YP_PARAM_MAX_WHEEL_ANGLE, MOTOR_RIGHT))
      r = p(YP_PARAM_MAX_WHEEL_ANGLE, MOTOR_RIGHT);
  }
  if (isset_p(YP_PARAM_MAX_WHEEL_ANGLE, MOTOR_LEFT))
  {
    if (l > p(YP_PARAM_MAX_WHEEL_ANGLE, MOTOR_LEFT))
      l = p(YP_PARAM_MAX_WHEEL_ANGLE, MOTOR_LEFT);
  }

  spur->wheel_angle[1] = l;
  spur->wheel_angle[0] = r;
  spur->run_mode = RUN_WHEEL_ANGLE;
}
