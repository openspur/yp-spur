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
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif  // HAVE_CONFIG_H

/* ボディパラメータ */
#include <shvel-param.h>

/* yp-spur用 */
#include <communication.h>
#include <serial.h>
#include <param.h>
#include <control.h>
#include <command.h>
#include <utility.h>
#include <yprintf.h>
#include <formula-calc.h>

/* ライブラリ用 */
#include <ypspur.h>

#include <pthread.h>

double g_P[YP_PARAM_NUM][YP_PARAM_MAX_MOTOR_NUM];
int g_P_changed[YP_PARAM_NUM][YP_PARAM_MAX_MOTOR_NUM];
int g_P_set[YP_PARAM_NUM][YP_PARAM_MAX_MOTOR_NUM];
struct rpf_t *g_Pf[YP_PARAM_NUM][YP_PARAM_MAX_MOTOR_NUM];
char g_state[YP_STATE_NUM];
Parameters g_param;
int g_param_init = 1;

int state(YPSpur_state id)
{
  return (g_state[id] == ENABLE);
}

void enable_state(YPSpur_state id)
{
  g_state[id] = ENABLE;
}

void disable_state(YPSpur_state id)
{
  g_state[id] = DISABLE;
}

double p(YPSpur_param id, enum motor_id motor)
{
  return g_P[id][motor];
}

int isset_p(YPSpur_param id, enum motor_id motor)
{
  return g_P_set[id][motor];
}

int ischanged_p(YPSpur_param id, enum motor_id motor)
{
  return g_P_changed[id][motor];
}

double *pp(YPSpur_param id, enum motor_id motor)
{
  return &g_P[id][motor];
}

int option(ParamOptions option)
{
  if (g_param.option & option)
    return 1;
  return 0;
}

ParamOutputLv output_lv(void)
{
  return g_param.output_lv;
}

ParametersPtr get_param_ptr()
{
  return &g_param;
}

int is_character(int c);
int is_number(int c);

/* 引数の説明 */
void arg_help(int argc, char *argv[])
{
  fprintf(stderr, "USAGE: %s [OPTION]...\n\n", argv[0]);
  fprintf(stderr, "  -v, --version            Display version info and exit.\n");
  fprintf(stderr, "  -d, --device <device>    Specify a B-Loco device. e.g. /dev/ttyUSB0\n");
  fprintf(stderr, "  -p, --param <tile>       Specify a robot parameter file.\n");
  fprintf(stderr, "  --verbose                Display detail information.\n");
  fprintf(stderr, "  --quiet                  Display nothing.\n");
  fprintf(stderr, "  -h, --help               This help\n");
  fprintf(stderr, "  --long-help              Long help\n");
  fprintf(stderr, "  --param-help             Comments of parameters\n\n");
}

/* 隠しコマンドの説明 */
void arg_longhelp(int argc, char *argv[])
{
  arg_help(argc, argv);
  fprintf(stderr, "  -o, --show-odometry      Display estimated robot position.\n");
  fprintf(stderr, "  -t, --show-timestamp     Display timestamp of serial communication \n"
                  "                            with the B-Loco device.\n");
  fprintf(stderr, "  --reconnect              Try reconnect device when device was closed.\n");
  fprintf(stderr, "  --without-ssm            Run without ssm output.\n");
  fprintf(stderr, "  -q, --msq-key <MSQKEY>   Run with message que key MSQKEY (default = 28741).\n");
  fprintf(stderr, "  -s, --speed <BAUDRATE>   Set baudrate.\n");
  fprintf(stderr, "  --admask <ADMASK>        Get AD data of ADMASK from B-Loco device.\n");
  fprintf(stderr, "  --enable-get-digital-io  Enable digital IO port.\n");
  fprintf(stderr, "  -c, --without-control    Run without control.\n");
  fprintf(stderr, "  --without-device         Run without B-Loco device.\n");
  fprintf(stderr, "  --no-yp-protocol         Run without checking plotocol of B-Loco device.\n");
  fprintf(stderr, "  --passive                Passive run mode.\n");
  fprintf(stderr, "  --update-param           Automatically reload parameter file.\n");
  fprintf(stderr, "  --high-resolution[=ARG]  Use high resolution velocity control mode (default = yes).\n");
  fprintf(stderr, "  --ssm-id <SSMID>         Change ssm id (default = 0).\n");
  fprintf(stderr, "  --socket <port>          Use socket ipc.\n");
  fprintf(stderr, "  --daemon                 Run in daemon mode.\n");
  fprintf(stderr, "  --ping                   Ping RS485 chained devices.\n");
}

/* 引数の説明 */
void param_help(void)
{
  int i;
  char param_names[YP_PARAM_NUM][32] = YP_PARAM_NAME;
  char param_comments[YP_PARAM_NUM][128] = YP_PARAM_COMMENT;

  fprintf(stderr, "INFO: Comments of parameters (parameter version %.1f)\n\n", YP_PARAM_REQUIRED_VERSION);
  for (i = 0; i < YP_PARAM_NUM; i++)
  {
    if (param_names[i][0] == '_')
      continue;
    fprintf(stderr, "  %20s: %s\n", param_names[i], param_comments[i]);
  }
}

/* 引数の解析 */
int arg_analyze(int argc, char *argv[])
{
  int i;

  g_param.option = OPTION_DEFAULT;
  g_param.msq_key = YPSPUR_MSQ_KEY;
  g_param.output_lv = OUTPUT_LV_INFO;
  g_param.speed = 0;
  g_param.ssm_id = 0;

  strcpy(g_param.parameter_filename, DEFAULT_PARAMETER_FILE);
  strcpy(g_param.device_name, DEFAULT_DEVICE_NAME);

  for (i = 1; i < argc; i++)
  {
    if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h"))
    {
      g_param.option |= OPTION_SHOW_HELP;
    }
    else if (!strcmp(argv[i], "--long-help"))
    {
      g_param.option |= OPTION_SHOW_LONGHELP;
    }
    else if (!strcmp(argv[i], "--daemon"))
    {
      g_param.option |= OPTION_DAEMON;
    }
    else if (!strcmp(argv[i], "--ping"))
    {
      g_param.option |= OPTION_PING;
    }
    else if (!strcmp(argv[i], "--param-help"))
    {
      g_param.option |= OPTION_SHOW_PARAMHELP;
    }
    else if (!strcmp(argv[i], "--show-odometry") || !strcmp(argv[i], "-o"))
    {
      g_param.option |= OPTION_SHOW_ODOMETRY;
    }
    else if (!strcmp(argv[i], "--show-timestamp") || !strcmp(argv[i], "-t"))
    {
      g_param.option |= OPTION_SHOW_TIMESTAMP;
    }
    else if (!strcmp(argv[i], "--param") || !strcmp(argv[i], "-p"))
    {
      if (i + 1 < argc)
      {
        i++;
        strcpy(g_param.parameter_filename, argv[i]);
        g_param.option |= OPTION_PARAM_FILE;
      }
      else
        break;
    }
    else if (!strcmp(argv[i], "--socket"))
    {
      if (i + 1 < argc)
      {
        i++;
        g_param.port = atoi(argv[i]);
        g_param.option |= OPTION_SOCKET;
      }
      else
        break;
    }
    else if (!strcmp(argv[i], "--admask"))
    {
      if (i + 1 < argc)
      {
        char *pos;

        i++;
        g_param.admask = 0;
        for (pos = argv[i]; *pos != 0; pos++)
        {
          g_param.admask = g_param.admask << 1;
          if (*pos == '1')
            g_param.admask |= 1;
        }
      }
      else
        break;
    }
    else if (!strcmp(argv[i], "--device") || !strcmp(argv[i], "-d"))
    {
      if (i + 1 < argc)
      {
        i++;
        strcpy(g_param.device_name, argv[i]);
      }
      else
        break;
    }
    else if (!strcmp(argv[i], "--without-control") || !strcmp(argv[i], "-c"))
    {
      g_param.option |= OPTION_PARAM_CONTROL;
      disable_state(YP_STATE_MOTOR);
      disable_state(YP_STATE_VELOCITY);
      disable_state(YP_STATE_BODY);
      disable_state(YP_STATE_TRACKING);
    }
    else if (!strcmp(argv[i], "--without-device"))
    {
      g_param.option |= OPTION_WITHOUT_DEVICE;
      g_param.option |= OPTION_DO_NOT_USE_YP;
      g_param.option |= OPTION_PARAM_CONTROL;
      disable_state(YP_STATE_MOTOR);
      disable_state(YP_STATE_VELOCITY);
      disable_state(YP_STATE_BODY);
      disable_state(YP_STATE_TRACKING);
    }
    else if (!strcmp(argv[i], "--high-resolution") ||
             !strcmp(argv[i], "--high-resolution=yes"))
    {
      g_param.option |= OPTION_HIGH_PREC;
    }
    else if (!strcmp(argv[i], "--high-resolution=no"))
    {
      g_param.option &= ~OPTION_HIGH_PREC;
    }
    else if (!strcmp(argv[i], "--version") || !strcmp(argv[i], "-v"))
    {
      g_param.option |= OPTION_VERSION;
    }
    else if (!strcmp(argv[i], "--verbose"))
    {
      g_param.output_lv = OUTPUT_LV_DEBUG;
    }
    else if (!strcmp(argv[i], "--quiet"))
    {
      g_param.output_lv = OUTPUT_LV_WARNING;
    }
    else if (!strcmp(argv[i], "--reconnect"))
    {
      g_param.option |= OPTION_RECONNECT;
    }
    else if (!strcmp(argv[i], "--enable-set-bs"))
    {
      g_param.option |= OPTION_ENABLE_SET_BS;
    }
    else if (!strcmp(argv[i], "--enable-get-digital-io"))
    {
      g_param.option |= OPTION_ENABLE_GET_DIGITAL_IO;
    }
    else if (!strcmp(argv[i], "--no-yp-protocol"))
    {
      g_param.option |= OPTION_DO_NOT_USE_YP;
    }
    else if (!strcmp(argv[i], "--without-ssm"))
    {
      g_param.option |= OPTION_WITHOUT_SSM;
    }
    else if (!strcmp(argv[i], "--msq-key") || !strcmp(argv[i], "-q"))
    {
      if (i + 1 < argc)
      {
        i++;
        g_param.msq_key = atoi(argv[i]);
      }
      else
        break;
    }
    else if (!strcmp(argv[i], "--speed") || !strcmp(argv[i], "-s"))
    {
      if (i + 1 < argc)
      {
        i++;
        g_param.speed = atoi(argv[i]);
      }
      else
        break;
    }
    else if (!strcmp(argv[i], "--passive"))
    {
      g_param.option |= OPTION_PASSIVE;
    }
    else if (!strcmp(argv[i], "--update-param"))
    {
      g_param.option |= OPTION_UPDATE_PARAM;
    }
    else if (!strcmp(argv[i], "--ssm-id"))
    {
      if (i + 1 < argc)
      {
        i++;
        g_param.ssm_id = atoi(argv[i]);
      }
      else
      {
        break;
      }
    }
    else
    {
      yprintf(OUTPUT_LV_ERROR, "ERROR : invalid option -- '%s'.\n", argv[i]);
      return 0;
    }
  }

  if (i < argc)
  {
    yprintf(OUTPUT_LV_ERROR, "ERROR :  option requires an argument -- '%s'.\n", argv[i]);
    return 0;
  }

  return 1;
}

/* parameter set command */
int parameter_set(char param, char id, long long int value64)
{
  char buf[7];
  int value;

  if (value64 > 0x7FFFFFFF || value64 < -0x7FFFFFFE)
  {
    yprintf(OUTPUT_LV_ERROR, "ERROR: parameter out of range (id: %d)\n", param);
    return -1;
  }
  value = value64;

  if (option(OPTION_WITHOUT_DEVICE))
  {
    return (0);
  }

  buf[0] = param;
  buf[1] = id;
  buf[2] = ((Int_4Char)value).byte[3];
  buf[3] = ((Int_4Char)value).byte[2];
  buf[4] = ((Int_4Char)value).byte[1];
  buf[5] = ((Int_4Char)value).byte[0];
  encode_write(buf, 6);

  return (0);
}

int is_character(int c)
{
  if (c >= 'A' && c <= 'Z')
    return 1;
  if (c >= 'a' && c <= 'z')
    return 1;
  if (c == '_')
    return 1;
  return 0;
}

int is_number(int c)
{
  if (c >= '0' && c <= '9')
    return 1;
  if (c == '-')
    return 1;
  if (c == '.')
    return 1;
  return 0;
}

void param_calc()
{
  int i, j;
  for (i = 0; i < YP_PARAM_NUM; i++)
  {
    for (j = 0; j < YP_PARAM_MAX_MOTOR_NUM; j++)
    {
      if (!g_param.motor_enable[j])
        continue;
      if (g_Pf[i][j])
      {
        double d;
        d = formula_eval(g_Pf[i][j]);
        yprintf(OUTPUT_LV_DEBUG, "Evaluated: [%d][%d] %f\n", i, j, d);
      }
    }
  }
}

/* パラメータファイルからの読み込み */
int set_paramptr(FILE *paramfile)
{
  char param_names[YP_PARAM_NUM][20] = YP_PARAM_NAME;
  char param_names0[YP_PARAM_NUM][24] = YP_PARAM_NAME;
  char param_names1[YP_PARAM_NUM][24] = YP_PARAM_NAME;
  int param_necessary[YP_PARAM_NUM] = YP_PARAM_NECESSARY;
#define VARIABLE_NUM 37
  char variable_names[VARIABLE_NUM][20] =
      {
        "X", "Y", "THETA", "V", "W",
        "WHEEL_VEL[0]", "WHEEL_VEL[1]",
        "WHEEL_VEL[2]", "WHEEL_VEL[3]",
        "WHEEL_VEL[4]", "WHEEL_VEL[5]",
        "WHEEL_VEL[6]", "WHEEL_VEL[7]",
        "WHEEL_VEL[8]", "WHEEL_VEL[9]",
        "WHEEL_VEL[10]", "WHEEL_VEL[11]",
        "WHEEL_VEL[12]", "WHEEL_VEL[13]",
        "WHEEL_VEL[14]", "WHEEL_VEL[15]",
        "WHEEL_ANGLE[0]", "WHEEL_ANGLE[1]",
        "WHEEL_ANGLE[2]", "WHEEL_ANGLE[3]",
        "WHEEL_ANGLE[4]", "WHEEL_ANGLE[5]",
        "WHEEL_ANGLE[6]", "WHEEL_ANGLE[7]",
        "WHEEL_ANGLE[8]", "WHEEL_ANGLE[9]",
        "WHEEL_ANGLE[10]", "WHEEL_ANGLE[11]",
        "WHEEL_ANGLE[12]", "WHEEL_ANGLE[13]",
        "WHEEL_ANGLE[14]", "WHEEL_ANGLE[15]",
      };
  struct variables_t variables[YP_PARAM_NUM * 3 + 1 + VARIABLE_NUM];
  struct
  {
    YPSpur_param alias;
    YPSpur_param param;
    int motor;
  } param_alias[YP_PARAM_ALIAS_NUM] = YP_PARAM_ALIAS;
  char name[32], value_str[100];
  int i, j, c;
  int str_wp;
  int read_state;
  int param_num, motor_num;
  OdometryPtr odm;
  int param_error = 0;

  for (i = 0; i < YP_PARAM_NUM; i++)
  {
    strcat(param_names0[i], "[0]");
    strcat(param_names1[i], "[1]");
    variables[i * 3 + 0].name = param_names[i];
    variables[i * 3 + 0].pointer = &g_P[i][0];
    variables[i * 3 + 1].name = param_names0[i];
    variables[i * 3 + 1].pointer = &g_P[i][0];
    variables[i * 3 + 2].name = param_names1[i];
    variables[i * 3 + 2].pointer = &g_P[i][1];
  }
  i = i * 3;
  for (j = 0; j < VARIABLE_NUM; j++)
  {
    variables[i + j].name = variable_names[j];
  }
  odm = get_odometry_ptr();
  variables[i++].pointer = &odm->x;
  variables[i++].pointer = &odm->y;
  variables[i++].pointer = &odm->theta;
  variables[i++].pointer = &odm->v;
  variables[i++].pointer = &odm->w;
  for (j = 0; j < 16; j++)
    variables[i++].pointer = &odm->wvel[j];
  for (j = 0; j < 16; j++)
    variables[i++].pointer = &odm->wang[j];
  variables[i].name = NULL;
  variables[i].pointer = NULL;

  for (i = 0; i < YP_PARAM_NUM; i++)
  {
    int j;
    for (j = 0; j < YP_PARAM_MAX_MOTOR_NUM; j++)
    {
      g_P_changed[i][j] = 0;
    }
  }
  if (g_param_init)
  {
    int j;
    for (j = 0; j < YP_PARAM_MAX_MOTOR_NUM; j++)
    {
      g_P_changed[YP_PARAM_PWM_MAX][j] = 1;
      g_param.motor_enable[j] = 0;
    }
    // パラメータの初期化
    for (i = 0; i < YP_PARAM_NUM; i++)
    {
      for (j = 0; j < YP_PARAM_MAX_MOTOR_NUM; j++)
      {
        g_P[i][j] = 0;
        g_P_set[i][j] = 0;
        g_Pf[i][j] = NULL;
      }
    }
  }

  // パラメータファイルの読み込み
  read_state = 0;
  str_wp = 0;
  motor_num = 0;
  param_num = YP_PARAM_NUM;
  while (1)
  {
    int eof = 0;

    c = getc(paramfile);
    if (c == EOF)
    {
      eof = 1;
      c = '\n';
    }

    switch (read_state)
    {
      case 0:
        /* - */
        if (c == '#')
        {
          read_state = 1;
        }
        if (is_character(c) || c == '-')
        {
          name[0] = c;
          read_state = 2;
          str_wp = 1;
        }
        break;
      case 1: /* comment */
        if (c == '\n')
        {
          read_state = 0;
        }
        break;
      case 2: /* name */
        name[str_wp] = c;
        if (!(is_character(c) || is_number(c) || c == '[' || c == ']' || c == '-'))
        {
          name[str_wp] = 0;
          read_state = 3;
          str_wp = 0;
        }
        else
        {
          str_wp++;
        }
        break;
      case 3: /* value */
        if (is_number(c))
        {
          str_wp = 0;
          value_str[str_wp] = c;
          str_wp++;
          read_state = 4;
        }
        else if (c == '=')
        {
          strcpy(value_str, name);
          strcat(value_str, "=");
          str_wp = strlen(value_str);
          read_state = 5;
        }
        if (read_state != 3)
        {
          char *num_start;
          char *num_end = NULL;
          int num;

          param_num = YP_PARAM_NUM;
          num = -1;

          num_start = strchr(name, '[');
          if (num_start)
          {
            num_start++;
            num_end = strchr(name, ']');
            if (!num_end)
            {
              read_state = 4;
              yprintf(OUTPUT_LV_ERROR, "Error: Invalid parameter -- '%s'.\n", name);
              param_error = 1;
              continue;
            }
            *(num_start - 1) = 0;
            *num_end = 0;
            num = atoi(num_start);
          }
          motor_num = num;
          for (i = 0; i < YP_PARAM_NUM; i++)
          {
            if (!strcmp(name, param_names[i]))
            {
              int j;
              for (j = 0; j < YP_PARAM_ALIAS_NUM; j++)
              {
                if (i == param_alias[j].alias)
                {
                  yprintf(OUTPUT_LV_WARNING, "Parameter alias: %s -> %s[%d]\n",
                          param_names[i], param_names[param_alias[j].param], param_alias[j].motor);
                  i = param_alias[j].param;
                  motor_num = param_alias[j].motor;
                  break;
                }
              }
              param_num = i;
              break;
            }
          }
          if (num_start)
          {
            *(num_start - 1) = '[';
            *num_end = ']';
          }
        }
        break;
      case 4: /* value */
        if (!is_number(c))
        {
          value_str[str_wp] = 0;
          str_wp = 0;
          if (param_num == YP_PARAM_NUM)
          {
            yprintf(OUTPUT_LV_ERROR, "Error: Invalid parameter -- '%s'.\n", name);
            param_error = 1;
          }
          else if (motor_num == -1)
          {
            int j;
            for (j = 0; j < YP_PARAM_MAX_MOTOR_NUM; j++)
            {
              g_P[param_num][j] = strtod(value_str, 0);
              g_P_set[param_num][j] = 1;
              g_P_changed[param_num][j] = 1;
              if (g_Pf[param_num][j])
              {
                formula_free(g_Pf[param_num][j]);
                g_Pf[param_num][j] = NULL;
              }
            }
            yprintf(OUTPUT_LV_DEBUG, "%d %s %f\n", param_num, param_names[param_num], g_P[param_num][0]);
          }
          else
          {
            g_P[param_num][motor_num] = strtod(value_str, 0);
            g_P_set[param_num][motor_num] = 1;
            g_P_changed[param_num][motor_num] = 1;
            g_param.motor_enable[motor_num] = 1;
            if (g_Pf[param_num][motor_num])
            {
              formula_free(g_Pf[param_num][motor_num]);
              g_Pf[param_num][motor_num] = NULL;
            }
            yprintf(OUTPUT_LV_DEBUG, "%d %s[%d] %f\n", param_num, param_names[param_num], motor_num,
                    g_P[param_num][motor_num]);
          }
          param_num = YP_PARAM_NUM;
          read_state = 0;
          str_wp = 0;
          break;
        }
        else
        {
          value_str[str_wp] = c;
          str_wp++;
        }
        break;
      case 5: /* value */
        if (c == '#' || c == '\n')
        {
          value_str[str_wp] = 0;
          str_wp = 0;
          if (param_num == YP_PARAM_NUM)
          {
            yprintf(OUTPUT_LV_ERROR, "Error: Invalid parameter -- '%s'.\n", name);
            param_error = 1;
          }
          else if (motor_num == -1)
          {
            int j;
            for (j = 0; j < YP_PARAM_MAX_MOTOR_NUM; j++)
            {
              g_P[param_num][j] = 0;
              g_P_set[param_num][j] = 1;
              if (g_Pf[param_num][j])
                formula_free(g_Pf[param_num][j]);
              formula(value_str, &g_Pf[param_num][j], variables);
            }
            if (g_Pf[param_num][0] == NULL)
            {
              yprintf(OUTPUT_LV_ERROR, "Error: Invalid formula -- '%s'.\n", value_str);
              param_error = 1;
            }
            else
            {
              for (j = 0; j < YP_PARAM_MAX_MOTOR_NUM; j++)
              {
                g_Pf[param_num][j] = formula_optimize(g_Pf[param_num][j]);
              }
              yprintf(OUTPUT_LV_DEBUG, "%d %s ", param_num, param_names[param_num]);
              if (output_lv() >= OUTPUT_LV_DEBUG)
                formula_print(stderr, g_Pf[param_num][0]);
              yprintf(OUTPUT_LV_DEBUG, "\n");
            }
          }
          else
          {
            g_P[param_num][motor_num] = 0;
            g_P_set[param_num][motor_num] = 1;
            if (g_Pf[param_num][motor_num])
              formula_free(g_Pf[param_num][motor_num]);
            formula(value_str, &g_Pf[param_num][motor_num], variables);
            if (g_Pf[param_num][motor_num] == NULL)
            {
              yprintf(OUTPUT_LV_ERROR, "Error: Invalid formula -- '%s'.\n", value_str);
              param_error = 1;
            }
            else
            {
              g_Pf[param_num][motor_num] = formula_optimize(g_Pf[param_num][motor_num]);
              yprintf(OUTPUT_LV_DEBUG, "%d %s[%d] ", param_num, param_names[param_num], motor_num);
              if (output_lv() >= OUTPUT_LV_DEBUG)
                formula_print(stderr, g_Pf[param_num][motor_num]);
              yprintf(OUTPUT_LV_DEBUG, "\n");
            }
          }
          param_num = YP_PARAM_NUM;
          read_state = 0;
          str_wp = 0;
          break;
        }
        else
        {
          value_str[str_wp] = c;
          str_wp++;
        }
        break;
    }
    if (eof)
      break;
  }

  fclose(paramfile);
  if (g_P[YP_PARAM_VERSION][0] < YP_PARAM_REQUIRED_VERSION)
  {
    yprintf(OUTPUT_LV_ERROR, "Error: Your parameter file format is too old!\n");
    yprintf(OUTPUT_LV_ERROR, "Error: This program requires %3.1f.\n", YP_PARAM_REQUIRED_VERSION);
    return 0;
  }
  if (g_P[YP_PARAM_VERSION][0] > YP_PARAM_SUPPORTED_VERSION)
  {
    yprintf(OUTPUT_LV_ERROR, "Error: Your parameter file format is unsupported!\n");
    yprintf(OUTPUT_LV_ERROR, "Error: Please install newer version of YP-Spur.\n");
    yprintf(OUTPUT_LV_ERROR, "Error: This program supports %3.1f.\n", YP_PARAM_SUPPORTED_VERSION);
    return 0;
  }

  for (i = 0; i < YP_PARAM_NUM; i++)
  {
    for (j = 0; j < YP_PARAM_MAX_MOTOR_NUM; j++)
    {
      if (!g_param.motor_enable[j])
        continue;
      if (param_necessary[i] && !g_P_set[i][j])
      {
        yprintf(OUTPUT_LV_ERROR, "Error: %s[%d] undefined!\n", param_names[i], j);
        param_error = 1;
      }
    }
  }
  if (param_error)
  {
    return 0;
  }

  g_param.num_motor_enable = 0;
  for (j = 0; j < YP_PARAM_MAX_MOTOR_NUM; j++)
  {
    // Count motor number
    if (g_param.motor_enable[j])
      g_param.num_motor_enable++;
  }
  if (g_param.num_motor_enable == 0)
  {
    // If all parameters are common among motors, treat num_motor_enable as two
    g_param.motor_enable[0] = g_param.motor_enable[1] = 1;
    g_param.num_motor_enable = 2;
  }

  for (j = 0; j < YP_PARAM_MAX_MOTOR_NUM; j++)
  {
    if (!g_param.motor_enable[j])
      continue;

    // Verify parameter version compatibility
    if (g_P_changed[YP_PARAM_ENCODER_DENOMINATOR][j] &&
        g_P_set[YP_PARAM_ENCODER_DENOMINATOR][j])
    {
      if (g_P[YP_PARAM_VERSION][0] < 5.0)
      {
        yprintf(
            OUTPUT_LV_ERROR,
            "ERROR: Using ENCODER_DENOMINATOR requires parameter version >= 5.0.\n"
            " Please make sure that all other motor parameters are based on mechanical motor revolution instead of electrical.\n");
        return 0;
      }
      yprintf(
          OUTPUT_LV_WARNING,
          "Warn: ENCODER_DENOMINATOR parameter support is experimental.\n"
          " The behavior of this parameter may be changed in the future update.\n");
    }

    // Check and fill undefined parameters
    if (!g_P_set[YP_PARAM_TORQUE_LIMIT][j])
    {
      yprintf(OUTPUT_LV_WARNING, "Warn: TORQUE_LIMIT[%d] undefined. TORQUE_MAX[%d] will be used.\n", j, j);
      g_P[YP_PARAM_TORQUE_LIMIT][j] = g_P[YP_PARAM_TORQUE_MAX][j];
      g_P_changed[YP_PARAM_TORQUE_LIMIT][j] = ischanged_p(YP_PARAM_TORQUE_MAX, j);
    }

    if (!g_P_set[YP_PARAM_VEHICLE_CONTROL][j])
    {
      double default_value;
      if (j < 2)
        default_value = 1.0;
      else
        default_value = 0.0;

      if (g_P[YP_PARAM_VEHICLE_CONTROL][j] != default_value)
        g_P_changed[YP_PARAM_VEHICLE_CONTROL][j] = 1;
      g_P[YP_PARAM_VEHICLE_CONTROL][j] = default_value;
    }

    if (!g_P_set[YP_PARAM_ENCODER_TYPE][j])
    {
      if (g_P[YP_PARAM_ENCODER_TYPE][j] != 2.0)
        g_P_changed[YP_PARAM_ENCODER_TYPE][j] = 1;
      g_P[YP_PARAM_ENCODER_TYPE][j] = 2.0;
    }
    if (!g_P_set[YP_PARAM_INDEX_GEAR][j])
    {
      if (g_P[YP_PARAM_INDEX_GEAR][j] != 1.0)
        g_P_changed[YP_PARAM_INDEX_GEAR][j] = 1;
      g_P[YP_PARAM_INDEX_GEAR][j] = 1.0;
    }
    if (!g_P_set[YP_PARAM_ENCODER_DENOMINATOR][j])
    {
      if (g_P[YP_PARAM_ENCODER_DENOMINATOR][j] != 1.0)
        g_P_changed[YP_PARAM_ENCODER_DENOMINATOR][j] = 1;
      g_P[YP_PARAM_ENCODER_DENOMINATOR][j] = 1.0;
    }
    if (!g_P_set[YP_PARAM_DEVICE_TIMEOUT][j])
    {
      // YP_PARAM_DEVICE_TIMEOUT is always sent on reloading.
      // g_P_changed is not needed to be marked.
      g_P[YP_PARAM_DEVICE_TIMEOUT][j] = 0.3;
    }

    // Process internally calculated parameters
    g_P[YP_PARAM_TORQUE_UNIT][j] = 1.0 / g_P[YP_PARAM_TORQUE_FINENESS][j];
    g_P_changed[YP_PARAM_TORQUE_UNIT][j] = ischanged_p(YP_PARAM_TORQUE_FINENESS, j);
  }

  // パラメータの指定によって自動的に求まるパラメータの計算
  calc_param_inertia2ff();

  /* パラメータを有効にする */
  enable_state(YP_STATE_MOTOR);
  enable_state(YP_STATE_VELOCITY);
  enable_state(YP_STATE_BODY);
  enable_state(YP_STATE_TRACKING);

  yprintf(OUTPUT_LV_DEBUG, "Info: %d motors defined\n", g_param.num_motor_enable);

  return 1;
}

/* パラメータファイルからの読み込み */
int set_param(char *filename, char *concrete_path)
{
  FILE *paramfile;
  paramfile = fopen(filename, "r");

  if (!paramfile)
  {
#if HAVE_PKG_CONFIG
    char dir_name[256];
    char file_name[256];
    char *pret;
    FILE *fd;
#endif  // HAVE_PKG_CONFIG

    yprintf(OUTPUT_LV_DEBUG, "Warn: File [%s] is not exist.\n", filename);

#if HAVE_PKG_CONFIG
    if (!strchr(filename, '/'))
    {
      /* ファイルが見つからないとき、かつパス指定でないときshareディレクトリを見に行く 
			*/
      fd = popen("pkg-config --variable=YP_PARAMS_DIR yp-robot-params", "r");
      if ((fd == NULL))
      {
        yprintf(OUTPUT_LV_ERROR,
                "Error: Cannot open pipe 'pkg-config --variable=YP_PARAMS_DIR yp-robot-params'.\n");
        return 0;
      }
      pret = fgets(dir_name, sizeof(dir_name), fd);

      if ((pclose(fd) == 0) && (pret != 0))
      {
        dir_name[strlen(dir_name) - 1] = '\0';
        // printf( "dir = '%s'\n", dir_name );
        snprintf(file_name, sizeof(file_name), "%s/%s", dir_name, filename);

        paramfile = fopen(file_name, "r");

        if (!paramfile)
        {
          yprintf(OUTPUT_LV_WARNING, "Warn: File [%s] is not exist.\n", file_name);
          return 0;
        }
      }
      else
      {
        yprintf(OUTPUT_LV_ERROR,
                "Error: Cannot read pipe 'pkg-config --variable=YP_PARAMS_DIR yp-robot-params'.\n");
        return 0;
      }
      // fprintf( stdout, "open %s\n", file_name );
      if (concrete_path)
        strcpy(concrete_path, file_name);
      strcpy(file_name, filename);
    }
    else
#endif  // HAVE_PKG_CONFIG
    {
      return 0;
    }
  }
  else
  {
    if (concrete_path)
      strcpy(concrete_path, filename);
  }

  return set_paramptr(paramfile);
}

void init_param_update_thread(pthread_t *thread, char *filename)
{
  g_param.parameter_applying = 0;
  if (pthread_create(thread, NULL, (void *)param_update, filename) != 0)
  {
    yprintf(OUTPUT_LV_ERROR, "Can't create command thread\n");
  }
}

void param_update_loop_cleanup(void *data)
{
  yprintf(OUTPUT_LV_INFO, "Parameter updater stopped.\n");
}

void param_update(void *filename)
{
  struct stat prev_status;

  yprintf(OUTPUT_LV_INFO, "Parameter updater started. Watching %s\n", filename);
  pthread_cleanup_push(param_update_loop_cleanup, filename);

  stat(filename, &prev_status);

  while (1)
  {
    int i;
    struct stat status;

    // 0.3秒に一回パラメータファイル更新をチェック
    for (i = 0; i < 3; i++)
    {
      yp_usleep(100000);
      pthread_testcancel();
    }
    g_param.parameter_applying = 0;
    stat(filename, &status);

    if (difftime(status.st_mtime, prev_status.st_mtime) != 0.0)
    {
      yp_usleep(100000);
      set_param(filename, NULL);
      if (!(option(OPTION_PARAM_CONTROL)))
      {
        g_param.parameter_applying = 1;
        apply_robot_params();
      }
    }

    prev_status = status;
  }
  pthread_cleanup_pop(1);
}

/* パラメータ適用 */
int apply_robot_params()
{
  yprintf(OUTPUT_LV_INFO, "Applying parameters.\n");

  int j;
  // ウォッチドックタイマの設定
  for (j = 0; j < YP_PARAM_MAX_MOTOR_NUM; j++)
  {
    if (g_param.motor_enable[j])
      parameter_set(PARAM_watch_dog_limit, j, 1200);
  }

  if (g_param_init)
  {
    for (j = 0; j < YP_PARAM_MAX_MOTOR_NUM; j++)
    {
      if (g_param.motor_enable[j])
        parameter_set(PARAM_w_ref, j, 0);
    }
    g_param_init = 0;
  }

  if (g_param.device_version >= 10)
  {
    int version, age;
    sscanf(YP_PROTOCOL_NAME, "YPP:%d:%d", &version, &age);
    for (j = 0; j < YP_PARAM_MAX_MOTOR_NUM; j++)
    {
      if (g_param.motor_enable[j])
        parameter_set(PARAM_protocol_version, j, version);
    }
  }

  /* モータのパラメータ */
  if (set_param_motor() < 1)
    return 0;
  yp_usleep(30000);

  /* 速度制御パラメータ */
  if (set_param_velocity() < 1)
    return 0;
  yp_usleep(100000);

  return 1;
}

// FF制御パラメータの計算
// 坪内　孝司　車輪移動体の制御　を参照
void calc_param_inertia2ff(void)
{
  int i;
  double A, B, C, D;  // 制御パラメータ
  double M, J;        // ロボットの質量、慣性モーメント
  double Gr, Gl;      // ギア比
  double Jmr, Jml;    // モータの慣性モーメント
  double Jtr, Jtl;    // タイヤの慣性モーメント
  double Rr, Rl;      // タイヤ半径
  double T;           // トレッド

  // パラメータの代入
  M = g_P[YP_PARAM_MASS][0];
  J = g_P[YP_PARAM_MOMENT_INERTIA][0];
  Gr = fabs(g_P[YP_PARAM_GEAR][0]);
  Gl = fabs(g_P[YP_PARAM_GEAR][1]);
  Jmr = g_P[YP_PARAM_MOTOR_M_INERTIA][0];
  Jml = g_P[YP_PARAM_MOTOR_M_INERTIA][1];
  Jtr = g_P[YP_PARAM_TIRE_M_INERTIA][0];
  Jtl = g_P[YP_PARAM_TIRE_M_INERTIA][1];
  Rr = g_P[YP_PARAM_RADIUS][0];
  Rl = g_P[YP_PARAM_RADIUS][1];
  T = g_P[YP_PARAM_TREAD][0];

  // パラメータの計算
  A = (Gr * Gr * Jmr + Jtr + Rr * Rr / 2.0 * (M / 2.0 + J / (T * T))) / Gr;
  B = (Gl * Gl * Jml + Jtl + Rl * Rl / 2.0 * (M / 2.0 + J / (T * T))) / Gl;
  C = (Rr * Rl / 2.0 * (M / 2.0 - J / (T * T))) / Gr;
  D = (Rr * Rl / 2.0 * (M / 2.0 - J / (T * T))) / Gl;

  // パラメータの設定
  g_P[YP_PARAM_GAIN_A][0] = A;
  g_P[YP_PARAM_GAIN_B][0] = B;
  g_P[YP_PARAM_GAIN_C][0] = C;
  g_P[YP_PARAM_GAIN_D][0] = D;

  g_P[YP_PARAM_INERTIA_SELF][0] = A;
  g_P[YP_PARAM_INERTIA_SELF][1] = B;
  g_P[YP_PARAM_INERTIA_CROSS][0] = C;
  g_P[YP_PARAM_INERTIA_CROSS][1] = D;

  if (ischanged_p(YP_PARAM_MASS, 0) ||
      ischanged_p(YP_PARAM_MOMENT_INERTIA, 0) ||
      ischanged_p(YP_PARAM_GEAR, 0) ||
      ischanged_p(YP_PARAM_GEAR, 1) ||
      ischanged_p(YP_PARAM_MOTOR_M_INERTIA, 0) ||
      ischanged_p(YP_PARAM_MOTOR_M_INERTIA, 1) ||
      ischanged_p(YP_PARAM_TIRE_M_INERTIA, 0) ||
      ischanged_p(YP_PARAM_TIRE_M_INERTIA, 1) ||
      ischanged_p(YP_PARAM_RADIUS, 0) ||
      ischanged_p(YP_PARAM_RADIUS, 1) ||
      ischanged_p(YP_PARAM_TREAD, 0))
  {
    g_P_changed[YP_PARAM_GAIN_A][0] = 1;
    g_P_changed[YP_PARAM_GAIN_B][0] = 1;
    g_P_changed[YP_PARAM_GAIN_C][0] = 1;
    g_P_changed[YP_PARAM_GAIN_D][0] = 1;
    g_P_changed[YP_PARAM_INERTIA_SELF][0] = 1;
    g_P_changed[YP_PARAM_INERTIA_CROSS][0] = 1;
    g_P_changed[YP_PARAM_INERTIA_SELF][1] = 1;
    g_P_changed[YP_PARAM_INERTIA_CROSS][1] = 1;
  }
  for (i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i++)
  {
    if (!g_param.motor_enable[i])
      continue;
    if (p(YP_PARAM_VEHICLE_CONTROL, i) > 0)
      continue;

    g_P[YP_PARAM_INERTIA_SELF][i] =
        fabs(g_P[YP_PARAM_GEAR][i]) * g_P[YP_PARAM_MOTOR_M_INERTIA][i] + g_P[YP_PARAM_TIRE_M_INERTIA][i] / fabs(g_P[YP_PARAM_GEAR][i]);
    g_P[YP_PARAM_INERTIA_CROSS][i] = 0;

    if (ischanged_p(YP_PARAM_GEAR, i) ||
        ischanged_p(YP_PARAM_MOTOR_M_INERTIA, i) ||
        ischanged_p(YP_PARAM_TIRE_M_INERTIA, i))
    {
      g_P_changed[YP_PARAM_INERTIA_SELF][i] = 1;
      g_P_changed[YP_PARAM_INERTIA_CROSS][i] = 1;
    }
  }
  // 出力（デバッグ）
  for (i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i++)
  {
    if (!g_param.motor_enable[i])
      continue;

    yprintf(OUTPUT_LV_DEBUG, " LOAD_INERTIA_SELF[%d]  %f\n", i, g_P[YP_PARAM_INERTIA_SELF][i]);
    yprintf(OUTPUT_LV_DEBUG, " LOAD_INERTIA_CROSS[%d] %f\n", i, g_P[YP_PARAM_INERTIA_CROSS][i]);
  }
}

// モータパラメータの送信
int set_param_motor(void)
{
  int j;
  // モータのパラメータ
  for (j = 0; j < YP_PARAM_MAX_MOTOR_NUM; j++)
  {
    if (!g_param.motor_enable[j])
      continue;
    if (ischanged_p(YP_PARAM_VOLT, j))
    {
      parameter_set(PARAM_vsrc, j, g_P[YP_PARAM_VOLT][j] * 256);
    }
    if (ischanged_p(YP_PARAM_VOLT_MIN, j) && isset_p(YP_PARAM_VOLT_MIN, j))
    {
      parameter_set(PARAM_vmin, j, g_P[YP_PARAM_VOLT_MIN][j] * 256);
    }
    if (ischanged_p(YP_PARAM_CYCLE, j))
    {
      parameter_set(PARAM_control_cycle, j, g_P[YP_PARAM_CYCLE][j] * 1000);
    }
    if (ischanged_p(YP_PARAM_MOTOR_PHASE, j))
    {
      parameter_set(PARAM_motor_phase, j, g_P[YP_PARAM_MOTOR_PHASE][j]);
    }
    if (ischanged_p(YP_PARAM_PHASE_OFFSET, j) ||
        ischanged_p(YP_PARAM_ENCODER_DENOMINATOR, j) ||
        ischanged_p(YP_PARAM_COUNT_REV, j))
    {
      const double enc_rev_phase = g_P[YP_PARAM_COUNT_REV][j] / g_P[YP_PARAM_ENCODER_DENOMINATOR][j];
      parameter_set(PARAM_phase_offset, j,
                    g_P[YP_PARAM_PHASE_OFFSET][j] * enc_rev_phase / (2.0 * M_PI));
    }
    if (ischanged_p(YP_PARAM_ENCODER_TYPE, j))
    {
      parameter_set(PARAM_enc_type, j, g_P[YP_PARAM_ENCODER_TYPE][j]);
    }
    if (ischanged_p(YP_PARAM_ENCODER_DIV, j))
    {
      parameter_set(PARAM_enc_div, j, g_P[YP_PARAM_ENCODER_DIV][j]);
    }
    if (ischanged_p(YP_PARAM_PWM_MAX, j) ||
        ischanged_p(YP_PARAM_MOTOR_R, j) ||
        ischanged_p(YP_PARAM_TORQUE_UNIT, j) ||
        ischanged_p(YP_PARAM_MOTOR_TC, j) ||
        ischanged_p(YP_PARAM_VOLT, j))
    {
      // ki [pwmcnt/toqcnt] = (voltage unit [V/toqcnt]) * (PWM_MAX [pwmcnt]) / (VOLT [V])
      // voltage unit [V/toqcnt] = (current unit [A/toqcnt]) * (MOTOR_R [ohm])
      // current unit [A/toqcnt] = (1 [toqcnt]) / ((TORQUE_UNIT [toqcnt/Nm]) * (MOTOR_TC [Nm/A]))

      // ki [pwmcnt/toqcnt] = ((MOTOR_R [ohm]) * (PWM_MAX [pwmcnt])) / ((TORQUE_UNIT [toqcnt/Nm]) * (MOTOR_TC [Nm/A]) * (VOLT [V]))

      long long int ki;
      ki = (double)(65536.0 * g_P[YP_PARAM_PWM_MAX][j] * g_P[YP_PARAM_MOTOR_R][j] * g_P[YP_PARAM_ENCODER_DENOMINATOR][j] /
                    (g_P[YP_PARAM_TORQUE_UNIT][j] * g_P[YP_PARAM_MOTOR_TC][j] *
                     g_P[YP_PARAM_VOLT][j]));
      yprintf(OUTPUT_LV_DEBUG, "Info: TORQUE_CONSTANT[%d]: %d\n", j, (int)ki);
      if (ki == 0)
      {
        yprintf(OUTPUT_LV_ERROR, "ERROR: TORQUE_CONSTANT[%d] fixed point value underflow\n", j);
        yprintf(OUTPUT_LV_ERROR, "ERROR: Increase TORQUE_FINENESS[%d]\n", j);
      }
      parameter_set(PARAM_p_ki, j, ki);
    }

    if (ischanged_p(YP_PARAM_PWM_MAX, j) ||
        ischanged_p(YP_PARAM_MOTOR_VC, j) ||
        ischanged_p(YP_PARAM_COUNT_REV, j) ||
        ischanged_p(YP_PARAM_CYCLE, j) ||
        ischanged_p(YP_PARAM_VOLT, j))
    {
      // kv [(pwmcnt)/(cnt/ms)] = (vc native [V/(cnt/ms)]) * (PWM_MAX [pwmcnt]) / (VOLT [V])
      // vc native [V/(cnt/ms)] = (((60 [sec/min]) / (MOTOR_VC [(rev/min)/V]])) / (COUNT_REV [cnt/rev])) / (CYCLE [sec/ms])

      // kv [(pwmcnt)/(cnt/ms)] = ((60 [sec/min]) * (PWM_MAX [pwmcnt]))
      //                          / ((MOTOR_VC [(rev/min)/V]]) * (COUNT_REV [cnt/rev]) * (CYCLE [sec/ms]) * (VOLT [V]))

      parameter_set(PARAM_p_kv, j,
                    (double)(65536.0 * g_P[YP_PARAM_PWM_MAX][j] * 60.0 /
                             (g_P[YP_PARAM_MOTOR_VC][j] * g_P[YP_PARAM_VOLT][j] * g_P[YP_PARAM_COUNT_REV][j] *
                              g_P[YP_PARAM_CYCLE][j])));
    }

    // 摩擦補償
    if (ischanged_p(YP_PARAM_TORQUE_NEWTON, j) || ischanged_p(YP_PARAM_TORQUE_UNIT, j))
    {
      parameter_set(PARAM_p_fr_plus, j, g_P[YP_PARAM_TORQUE_NEWTON][j] * g_P[YP_PARAM_TORQUE_UNIT][j]);
    }

    if (g_P_set[YP_PARAM_TORQUE_NEWTON_NEG][j])
    {
      if (ischanged_p(YP_PARAM_TORQUE_NEWTON_NEG, j) || ischanged_p(YP_PARAM_TORQUE_UNIT, j))
      {
        parameter_set(PARAM_p_fr_minus, j, g_P[YP_PARAM_TORQUE_NEWTON_NEG][j] * g_P[YP_PARAM_TORQUE_UNIT][j]);
      }
    }
    else
    {
      if (ischanged_p(YP_PARAM_TORQUE_NEWTON, j) || ischanged_p(YP_PARAM_TORQUE_UNIT, j))
      {
        parameter_set(PARAM_p_fr_minus, j, g_P[YP_PARAM_TORQUE_NEWTON][j] * g_P[YP_PARAM_TORQUE_UNIT][j]);
      }
    }

    // cnt/ms -> rad/s = cnt/ms * ms/s * rad/cnt = cnt/ms * 2pi/COUNT_REV / CYCLE
    if (ischanged_p(YP_PARAM_COUNT_REV, j) ||
        ischanged_p(YP_PARAM_CYCLE, j))
    {
      const double tvc = (2.0 * M_PI / g_P[YP_PARAM_COUNT_REV][j]) / g_P[YP_PARAM_CYCLE][j];
      if (ischanged_p(YP_PARAM_TORQUE_VISCOS, j) || ischanged_p(YP_PARAM_TORQUE_UNIT, j))
      {
        parameter_set(PARAM_p_fr_wplus, j, g_P[YP_PARAM_TORQUE_VISCOS][j] * g_P[YP_PARAM_TORQUE_UNIT][j] * tvc);
      }
      if (g_P_set[YP_PARAM_TORQUE_NEWTON_NEG][j])
      {
        if (ischanged_p(YP_PARAM_TORQUE_VISCOS_NEG, j) || ischanged_p(YP_PARAM_TORQUE_UNIT, j))
        {
          parameter_set(PARAM_p_fr_wminus, j, g_P[YP_PARAM_TORQUE_VISCOS_NEG][j] * g_P[YP_PARAM_TORQUE_UNIT][j] * tvc);
        }
      }
      else
      {
        if (ischanged_p(YP_PARAM_TORQUE_VISCOS_NEG, j) || ischanged_p(YP_PARAM_TORQUE_UNIT, j))
        {
          parameter_set(PARAM_p_fr_wminus, j, g_P[YP_PARAM_TORQUE_VISCOS][j] * g_P[YP_PARAM_TORQUE_UNIT][j] * tvc);
        }
      }
    }

    if (ischanged_p(YP_PARAM_TORQUE_OFFSET, j) || ischanged_p(YP_PARAM_TORQUE_UNIT, j))
    {
      parameter_set(PARAM_p_toq_offset, j, g_P[YP_PARAM_TORQUE_OFFSET][j] * g_P[YP_PARAM_TORQUE_UNIT][j]);
    }

    if (ischanged_p(YP_PARAM_TORQUE_MAX, j) || ischanged_p(YP_PARAM_TORQUE_UNIT, j))
    {
      parameter_set(PARAM_toq_max, j, g_P[YP_PARAM_TORQUE_MAX][j] * g_P[YP_PARAM_TORQUE_UNIT][j]);
      parameter_set(PARAM_toq_min, j, -g_P[YP_PARAM_TORQUE_MAX][j] * g_P[YP_PARAM_TORQUE_UNIT][j]);
    }

    if (ischanged_p(YP_PARAM_TORQUE_LIMIT, j) || ischanged_p(YP_PARAM_TORQUE_UNIT, j))
    {
      parameter_set(PARAM_toq_limit, j, g_P[YP_PARAM_TORQUE_LIMIT][j] * g_P[YP_PARAM_TORQUE_UNIT][j]);
    }

    if (ischanged_p(YP_PARAM_PWM_MAX, j))
    {
      parameter_set(PARAM_pwm_max, j, g_P[YP_PARAM_PWM_MAX][j]);
      parameter_set(PARAM_pwm_min, j, -g_P[YP_PARAM_PWM_MAX][j]);
    }

    if (ischanged_p(YP_PARAM_COUNT_REV, j))
    {
      parameter_set(PARAM_enc_rev, j, g_P[YP_PARAM_COUNT_REV][j]);
    }
    if (ischanged_p(YP_PARAM_ENCODER_DENOMINATOR, j) &&
        isset_p(YP_PARAM_ENCODER_DENOMINATOR, j))
    {
      if (g_param.device_version < 10)
      {
        yprintf(OUTPUT_LV_ERROR, "ERROR: the device doesn't support ENCODER_DENOMINATOR\n");
        return 0;
      }
      parameter_set(PARAM_enc_denominator, j, g_P[YP_PARAM_ENCODER_DENOMINATOR][j]);
    }
    if (ischanged_p(YP_PARAM_HALL_DELAY, j) ||
        ischanged_p(YP_PARAM_CYCLE, j))
    {
      if (isset_p(YP_PARAM_HALL_DELAY, j))
      {
        if (g_param.device_version < 11)
        {
          yprintf(OUTPUT_LV_ERROR, "ERROR: the device doesn't support HALL_DELAY\n");
          return 0;
        }
        const float delay_factor = g_P[YP_PARAM_HALL_DELAY][j] / g_P[YP_PARAM_CYCLE][j];
        parameter_set(PARAM_hall_delay_factor, j, lroundf(32768.0 * delay_factor));
      }
    }
    if (ischanged_p(YP_PARAM_LR_CUTOFF_FREQ, j) ||
        ischanged_p(YP_PARAM_ENCODER_DENOMINATOR, j) ||
        ischanged_p(YP_PARAM_COUNT_REV, j) ||
        ischanged_p(YP_PARAM_CYCLE, j))
    {
      if (isset_p(YP_PARAM_LR_CUTOFF_FREQ, j))
      {
        if (g_param.device_version < 11)
        {
          yprintf(OUTPUT_LV_ERROR, "ERROR: the device doesn't support LR_CUTOFF_FREQ\n");
          return 0;
        }
        const float cutoff_vel =
            g_P[YP_PARAM_LR_CUTOFF_FREQ][j] * g_P[YP_PARAM_CYCLE][j] *
            g_P[YP_PARAM_COUNT_REV][j] / g_P[YP_PARAM_ENCODER_DENOMINATOR][j];
        parameter_set(PARAM_lr_cutoff_vel, j, lroundf(cutoff_vel));
      }
    }

    // Sleep to keep bandwidth margin
    yp_usleep(20000);
  }
  return 1;
}

int set_param_velocity(void)
{
  int j;

  if (g_param.device_version <= 6)
  {
    double ffr, ffl;
    int ffr_changed = 0, ffl_changed = 0;

    // Feed-forward dynamics compensation parameter
    // (torque [Nm]) = (wheel acc [rad/(s*s)]) * (wheel inertia [kgf*m*m])
    // (torque [toqcnt])
    //    = (wheel acc [cnt/(s*s)) * (TORQUE_UNIT [toqcnt/Nm] * (wheel inertia [kgf*m*m]) * (2 * pi) / ((COUNT_REV [cnt]) * (GEAR))
    // (acc-torque factor [toqcnt/(cnt/(s*s))])
    //    = (TORQUE_UNIT [toqcnt/Nm]) * (wheel inertia [kgf*m*m]) * (2 * pi) / ((COUNT_REV [cnt]) * (GEAR))

    if (ischanged_p(YP_PARAM_TORQUE_UNIT, 0) ||
        ischanged_p(YP_PARAM_COUNT_REV, 0) ||
        ischanged_p(YP_PARAM_GEAR, 0))
    {
      ffr_changed = 1;
    }
    ffr = 256.0 * 2.0 * M_PI * g_P[YP_PARAM_TORQUE_UNIT][0] / (g_P[YP_PARAM_COUNT_REV][0] * fabs(g_P[YP_PARAM_GEAR][0]));
    if (ischanged_p(YP_PARAM_GAIN_A, 0) || ffr_changed)
    {
      yprintf(OUTPUT_LV_DEBUG, "Info: GAIN_A: %d\n",
              (int)(g_P[YP_PARAM_GAIN_A][0] * ffr));
      if (abs(g_P[YP_PARAM_GAIN_A][0] * ffr) == 0)
      {
        yprintf(OUTPUT_LV_ERROR, "ERROR: GAIN_A fixed point value underflow\n");
        yprintf(OUTPUT_LV_ERROR, "ERROR: Decrease TORQUE_FINENESS[%d]\n", 0);
        return 0;
      }
      parameter_set(PARAM_p_A, 0, g_P[YP_PARAM_GAIN_A][0] * ffr);
    }
    if (ischanged_p(YP_PARAM_GAIN_C, 0) || ffr_changed)
      parameter_set(PARAM_p_C, 0, g_P[YP_PARAM_GAIN_C][0] * ffr);
    if (ischanged_p(YP_PARAM_GAIN_E, 0) || ffr_changed)
      parameter_set(PARAM_p_E, 0, g_P[YP_PARAM_GAIN_E][0] * ffr);

    if (ischanged_p(YP_PARAM_TORQUE_UNIT, 1) ||
        ischanged_p(YP_PARAM_COUNT_REV, 1) ||
        ischanged_p(YP_PARAM_GEAR, 1))
    {
      ffl_changed = 1;
    }
    ffl = 256.0 * 2.0 * M_PI * g_P[YP_PARAM_TORQUE_UNIT][1] / (g_P[YP_PARAM_COUNT_REV][1] * fabs(g_P[YP_PARAM_GEAR][1]));
    if (ischanged_p(YP_PARAM_GAIN_B, 0) || ffl_changed)
    {
      yprintf(OUTPUT_LV_DEBUG, "Info: GAIN_B: %d\n",
              (int)(g_P[YP_PARAM_GAIN_A][0] * ffl));
      if (abs(g_P[YP_PARAM_GAIN_B][0] * ffl) == 0)
      {
        yprintf(OUTPUT_LV_ERROR, "ERROR: GAIN_B fixed point value underflow\n");
        yprintf(OUTPUT_LV_ERROR, "ERROR: Decrease TORQUE_FINENESS[%d]\n", 1);
        return 0;
      }
      parameter_set(PARAM_p_B, 0, g_P[YP_PARAM_GAIN_B][0] * ffl);
    }
    if (ischanged_p(YP_PARAM_GAIN_D, 0) || ffl_changed)
      parameter_set(PARAM_p_D, 0, g_P[YP_PARAM_GAIN_D][0] * ffl);
    if (ischanged_p(YP_PARAM_GAIN_F, 0) || ffl_changed)
      parameter_set(PARAM_p_F, 0, g_P[YP_PARAM_GAIN_F][0] * ffl);
  }
  // PI制御のパラメータ
  for (j = 0; j < YP_PARAM_MAX_MOTOR_NUM; j++)
  {
    if (!g_param.motor_enable[j])
      continue;

    if (g_param.device_version > 6)
    {
      int ff_changed = 0;
      double ff = 256.0 * 2.0 * M_PI * g_P[YP_PARAM_TORQUE_UNIT][j] /
                  (g_P[YP_PARAM_COUNT_REV][j] * fabs(g_P[YP_PARAM_GEAR][j]));
      if (ischanged_p(YP_PARAM_TORQUE_UNIT, j) ||
          ischanged_p(YP_PARAM_COUNT_REV, j) ||
          ischanged_p(YP_PARAM_GEAR, j))
      {
        ff_changed = 1;
      }
      if (ischanged_p(YP_PARAM_INERTIA_SELF, j) || ff_changed)
      {
        yprintf(OUTPUT_LV_DEBUG, "Info: INERTIA_SELF[%d]: %d\n", j,
                (int)(g_P[YP_PARAM_INERTIA_SELF][j] * ff));
        if (abs(g_P[YP_PARAM_INERTIA_SELF][j] * ff) == 0)
        {
          yprintf(OUTPUT_LV_ERROR, "ERROR: INERTIA_SELF[%d] fixed point value underflow\n", j);
          yprintf(OUTPUT_LV_ERROR, "ERROR: Decrease TORQUE_FINENESS[%d]\n", j);
          return 0;
        }
        parameter_set(PARAM_p_inertia_self, j, g_P[YP_PARAM_INERTIA_SELF][j] * ff);
      }
      if (ischanged_p(YP_PARAM_INERTIA_CROSS, j) || ff_changed)
      {
        parameter_set(PARAM_p_inertia_cross, j, g_P[YP_PARAM_INERTIA_CROSS][j] * ff);
      }
    }

    // [1/s]
    if (ischanged_p(YP_PARAM_GAIN_KP, j))
      parameter_set(PARAM_p_pi_kp, j, g_P[YP_PARAM_GAIN_KP][j]);
    // [1/s^2]
    if (ischanged_p(YP_PARAM_GAIN_KI, j))
      parameter_set(PARAM_p_pi_ki, j, g_P[YP_PARAM_GAIN_KI][j]);
    // 各種制限
    if (ischanged_p(YP_PARAM_INTEGRAL_MAX, j) ||
        ischanged_p(YP_PARAM_COUNT_REV, j) ||
        ischanged_p(YP_PARAM_GEAR, j))
    {
      parameter_set(PARAM_int_max, j,
                    g_P[YP_PARAM_INTEGRAL_MAX][j] * g_P[YP_PARAM_COUNT_REV][j] * fabs(g_P[YP_PARAM_GEAR][j]));
      parameter_set(PARAM_int_min, j,
                    -g_P[YP_PARAM_INTEGRAL_MAX][j] * g_P[YP_PARAM_COUNT_REV][j] * fabs(g_P[YP_PARAM_GEAR][j]));
    }
  }

  // ウォッチドックタイマの設定
  for (j = 0; j < YP_PARAM_MAX_MOTOR_NUM; j++)
  {
    if (!g_param.motor_enable[j])
      continue;
    // [s] -> [ms]
    parameter_set(PARAM_watch_dog_limit, j, g_P[YP_PARAM_DEVICE_TIMEOUT][j] * 1000);
  }
  return 1;
}
