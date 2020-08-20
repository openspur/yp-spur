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
#include <adinput.h>
#include <param.h>
#include <odometry.h>

/* ライブラリ用 */
#include <ypspur.h>
#include <cartesian2d.h>

void get_ad_com(double *data, double *resdata)
{
  int num;

  num = (int)data[0];
  resdata[0] = get_addata(num);
}

void set_io_dir_com(double *data, double *resdata)
{
  int num;

  num = (int)data[0];
  parameter_set(PARAM_io_dir, 0, num);
}

void set_io_data_com(double *data, double *resdata)
{
  int num;

  num = (int)data[0];
  parameter_set(PARAM_io_data, 0, num);
}

void get_error_state_com(double *data, double *resdata)
{
  int num = (int)data[0];
  ErrorStatePtr err = get_error_state_ptr();

  resdata[0] = err->state[num];
  resdata[1] = err->time[num];
}

void dump_device_com(double *data, double *resdata)
{
  int id, block;

  id = (int)data[0];
  block = (int)data[1];
  parameter_set(PARAM_dump, id, block);
}

