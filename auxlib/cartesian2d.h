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

#ifndef __AUXLIB_CARTESIAN2D_H
#define __AUXLIB_CARTESIAN2D_H

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct cs_t* CSptr;
typedef struct cs_t
{
  double x;
  double y;
  double theta;
  CSptr parent;
  CSptr child;
  CSptr brother;
  int level;
} CoordinateSystem;

CSptr CS_add(CSptr parent_cs, double x, double y, double theta);
int CS_delete(CSptr target_cs);
int CS_set(CSptr target_cs, double x, double y, double theta);
int CS_set_on_CS(CSptr target_cs, CSptr on_cs, double x, double y, double theta);
void CS_turn_base(double* x, double* y, double* theta);

void CS_recursive_trans(CSptr target_cs, CSptr now_cs, double* x, double* y, double* theta);
void inv_trans_cs(CSptr target_cs, double* x, double* y, double* theta);
void trans_cs(CSptr target_cs, double* x, double* y, double* theta);
void trace_trans_cs(CSptr target_cs, double* x, double* y, double* theta);

#ifdef __cplusplus
}
#endif  // __cplusplus
#endif  // __AUXLIB_CARTESIAN2D_H
