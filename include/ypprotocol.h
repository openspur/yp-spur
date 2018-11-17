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

#ifndef YPPROTOCOL_H
#define YPPROTOCOL_H

#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/** Structure of VV command */
typedef struct VERSION_INFO
{
  char vender[128];
  char product[128];
  char firmware[128];
  char protocol[128];
  char serialno[128];
} Ver_t;

/** Structure of PP command */
typedef struct PARAMETER_INFO
{
  char pwm_resolution[128];
  char motor_num[128];
  char robot_name[128];
} Param_t;

int get_version(Ver_t *apVer);
int get_parameter(Param_t *apParam);
int set_baudrate(int baud);
int get_embedded_param(char *param);

#ifdef __cplusplus
}
#endif  // __cplusplus
#endif  // YPPROTOCOL_H
