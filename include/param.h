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

#ifndef PARAM_H
#define PARAM_H

#include <stdio.h>

#include <ypparam.h>
#include <utility.h>

#include <pthread.h>

typedef enum
{
  OPTION_SHOW_ODOMETRY = 0x0001,
  OPTION_PARAM_CONTROL = 0x0002,
  OPTION_SHOW_TIMESTAMP = 0x0004,
  OPTION_SHOW_HELP = 0x0008,
  OPTION_SHOW_LONGHELP = 0x0010,
  OPTION_SHOW_PARAMHELP = 0x0020,
  OPTION_VERSION = 0x0040,
  OPTION_DAEMON = 0x0080,

  OPTION_SOCKET = 0x00200,
  OPTION_PARAM_FILE = 0x00400,

  OPTION_WITHOUT_DEVICE = 0x00800,
  OPTION_WITHOUT_SSM = 0x01000,
  OPTION_ENABLE_SET_BS = 0x02000,
  OPTION_DO_NOT_USE_YP = 0x04000,
  OPTION_RECONNECT = 0x08000,
  OPTION_ENABLE_GET_DIGITAL_IO = 0x10000,
  OPTION_PASSIVE = 0x20000,
  OPTION_UPDATE_PARAM = 0x40000,
  OPTION_HIGH_PREC = 0x80000,
  OPTION_PING = 0x100000,
} ParamOptions;

#define OPTION_DEFAULT (OPTION_HIGH_PREC)

#define DEFAULT_PARAMETER_FILE "./robot.param"
#define DEFAULT_DEVICE_NAME "/dev/ttyUSB0"

#define GRAVITY 9.81
#define SIGN(x) ((x < 0) ? -1 : 1)

typedef enum
{
  OUTPUT_LV_ERROR,
  OUTPUT_LV_WARNING,
  OUTPUT_LV_INFO,
  OUTPUT_LV_DEBUG,
} ParamOutputLv;

typedef struct _parameters* ParametersPtr;
typedef struct _parameters
{
  char parameter_filename[132];
  char device_name[132];
  int msq_key;
  int port;
  int speed;
  ParamOptions option;
  ParamOutputLv output_lv;
  unsigned char admask;
  int ssm_id;
  int motor_enable[YP_PARAM_MAX_MOTOR_NUM];
  int num_motor_enable;
  int device_version;
  int device_version_age;
  int parameter_applying;
} Parameters;

int arg_analyze(int argc, char* argv[]);
void arg_help(int argc, char* argv[]);
void param_help(void);
void arg_longhelp(int argc, char* argv[]);
int set_param(char* filename, char* concrete_path);
int set_paramptr(FILE* paramfile);
void calc_param_inertia2ff(void);
int set_param_motor(void);
int set_param_velocity(void);
int parameter_set(char param, char id, long long int value64);
int apply_robot_params(void);

int does_option_set(ParamOptions option);
int state(YPSpur_state id);
void enable_state(YPSpur_state id);
void disable_state(YPSpur_state id);
double p(YPSpur_param id, enum motor_id motor);
int isset_p(YPSpur_param id, enum motor_id motor);
double* pp(YPSpur_param id, enum motor_id motor);
ParametersPtr get_param_ptr();
int option(ParamOptions option);
ParamOutputLv output_lv(void);
void param_calc();

void param_update(void* filename);
void init_param_update_thread(pthread_t* thread, char* filename);
void param_update_loop_cleanup(void* data);

#endif  // PARAM_H
