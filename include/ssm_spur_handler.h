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

#ifndef SSM_SPUR_HANDLER_H
#define SSM_SPUR_HANDLER_H

#include <command.h>

void init_ypspurSSM(int ssm_id);
void end_ypspurSSM(void);
void write_ypspurSSM(
    int odometry_updated, int receive_count,
    Odometry *odm_log, int readdata_num, Short_2Char *cnt1_log, Short_2Char *cnt2_log,
    Short_2Char *pwm1_log, Short_2Char *pwm2_log, int ad_log[][8]);
void coordinate_synchronize(Odometry *odm, SpurUserParamsPtr spur);

#endif  // SSM_SPUR_HANDLER_H
