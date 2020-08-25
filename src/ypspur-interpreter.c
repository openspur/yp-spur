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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
#include <getopt.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif  // HAVE_CONFIG_H

#if HAVE_LIBREADLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif  // HAVE_LIBREADLINE

#include <ypspur.h>
#include <utility.h>

typedef struct SPUR_COMMAND
{
  enum
  {
    SPUR_NONE = -1,
    SPUR_LINE,
    SPUR_STOP_LINE,
    SPUR_CIRCLE,
    SPUR_SPIN,
    SPUR_GETPOS,
    SPUR_GETVEL,
    SPUR_SETPOS,
    SPUR_GETFORCE,
    SPUR_NEAR_POS,
    SPUR_NEAR_ANG,
    SPUR_OVER_LINE,
    SPUR_ADJUSTPOS,
    SPUR_SETVEL,
    SPUR_SETANGVEL,
    SPUR_SETACCEL,
    SPUR_SETANGACCEL,
    SPUR_INIT,
    SPUR_FREE,
    SPUR_OPENFREE,
    SPUR_STOP,
    SPUR_FREEZE,
    SPUR_UNFREEZE,
    SPUR_ISFREEZE,
    SPUR_GETAD,
    SPUR_SET_IO_DIR,
    SPUR_SET_IO_DATA,
    SPUR_GET_DEVICE_ERROR_STATE,
    SPUR_VEL,
    SPUR_WHEEL_VEL,
    SPUR_GET_WHEEL_VEL,
    SPUR_GET_WHEEL_ANG,
    SPUR_GET_WHEEL_TORQUE,
    SPUR_WHEEL_TORQUE,
    SPUR_GET_VREF,
    SPUR_GET_WHEEL_VREF,
    SPUR_ORIENT,
    SPUR_SET_WHEEL_VEL,
    SPUR_SET_WHEEL_ACCEL,
    SPUR_WHEEL_ANG,
    SPUR_JOINT_TORQUE,
    SPUR_JOINT_VEL,
    SPUR_JOINT_ANG,
    SPUR_JOINT_ANG_VEL,
    SPUR_SET_JOINT_ACCEL,
    SPUR_SET_JOINT_VEL,
    SPUR_GET_JOINT_VEL,
    SPUR_GET_JOINT_VREF,
    SPUR_GET_JOINT_ANG,
    SPUR_GET_JOINT_TORQUE,
    SPUR_SLEEP,
    SPUR_REQUEST_DEVICE_DUMP,
    EXIT,
    HELP,
    SPUR_COMMAND_MAX
  } id;
  char name[64];
  int reqarg;
  int narg;
  double arg[3];
} SpurCommand;

static const SpurCommand SPUR_COMMAND[SPUR_COMMAND_MAX] = {
  { SPUR_LINE, { "line" }, 3 },
  { SPUR_STOP_LINE, { "stop_line" }, 3 },
  { SPUR_CIRCLE, { "circle" }, 3 },
  { SPUR_SPIN, { "spin" }, 1 },
  { SPUR_GETPOS, { "get_pos" }, 0 },
  { SPUR_GETVEL, { "get_vel" }, 0 },
  { SPUR_GETFORCE, { "get_force" }, 0 },
  { SPUR_SETPOS, { "set_pos" }, 3 },
  { SPUR_NEAR_POS, { "near_pos" }, 3 },
  { SPUR_NEAR_ANG, { "near_ang" }, 2 },
  { SPUR_OVER_LINE, { "over_line" }, 3 },
  { SPUR_ADJUSTPOS, { "adjust_pos" }, 3 },
  { SPUR_SETVEL, { "set_vel" }, 1 },
  { SPUR_SETANGVEL, { "set_angvel" }, 1 },
  { SPUR_SETACCEL, { "set_accel" }, 1 },
  { SPUR_SETANGACCEL, { "set_angaccel" }, 1 },
  { SPUR_FREE, { "free" }, 0 },
  { SPUR_OPENFREE, { "openfree" }, 0 },
  { SPUR_STOP, { "stop" }, 0 },
  { SPUR_INIT, { "init" }, 0 },
  { SPUR_FREEZE, { "freeze" }, 0 },
  { SPUR_UNFREEZE, { "unfreeze" }, 0 },
  { SPUR_ISFREEZE, { "isfreeze" }, 0 },
  { SPUR_GETAD, { "get_ad_value" }, 1 },
  { SPUR_SET_IO_DIR, { "set_io_dir" }, 1 },
  { SPUR_SET_IO_DATA, { "set_io_data" }, 1 },
  { SPUR_GET_DEVICE_ERROR_STATE, { "get_device_error_state" }, 1 },
  { SPUR_VEL, { "vel" }, 2 },
  { SPUR_WHEEL_VEL, { "wheel_vel" }, 2 },
  { SPUR_GET_WHEEL_VEL, { "get_wheel_vel" }, 0 },
  { SPUR_GET_WHEEL_ANG, { "get_wheel_ang" }, 0 },
  { SPUR_GET_WHEEL_TORQUE, { "get_wheel_torque" }, 0 },
  { SPUR_WHEEL_TORQUE, { "wheel_torque" }, 2 },
  { SPUR_ORIENT, { "orient" }, 1 },
  { SPUR_GET_VREF, { "get_vref" }, 0 },
  { SPUR_GET_WHEEL_VREF, { "get_wheel_vref" }, 0 },
  { SPUR_SET_WHEEL_VEL, { "set_wheel_vel" }, 2 },
  { SPUR_SET_WHEEL_ACCEL, { "set_wheel_accel" }, 2 },
  { SPUR_WHEEL_ANG, { "wheel_ang" }, 2 },
  { SPUR_JOINT_TORQUE, { "joint_torque" }, 2 },
  { SPUR_JOINT_VEL, { "joint_vel" }, 2 },
  { SPUR_JOINT_ANG, { "joint_ang" }, 2 },
  { SPUR_JOINT_ANG_VEL, { "joint_ang_vel" }, 3 },
  { SPUR_SET_JOINT_ACCEL, { "set_joint_accel" }, 2 },
  { SPUR_SET_JOINT_VEL, { "set_joint_vel" }, 2 },
  { SPUR_GET_JOINT_VEL, { "get_joint_vel" }, 1 },
  { SPUR_GET_JOINT_VREF, { "get_joint_vref" }, 1 },
  { SPUR_GET_JOINT_ANG, { "get_joint_ang" }, 1 },
  { SPUR_GET_JOINT_TORQUE, { "get_joint_torque" }, 1 },
  { SPUR_SLEEP, { "sleep" }, 1 },
  { SPUR_REQUEST_DEVICE_DUMP, { "request_device_dump" }, 2 },
  { HELP, { "help" }, 0 },
  { EXIT, { "exit" }, 0 }
};

#if HAVE_SIGLONGJMP
sigjmp_buf ctrlc_capture;
#endif  // HAVE_SIGLONGJMP

void ctrlc(int num)
{
  printf("\n");
#if HAVE_SIGLONGJMP
  siglongjmp(ctrlc_capture, 1);
#endif  // HAVE_SIGLONGJMP
}

int proc_spur_cmd(char *line, int *coordinate)
{
  int i;
  SpurCommand spur;
  int lcoordinate;
  char *argv;
  int ret;
  int ad;
  int error;
  enum
  {
    MODE_COMMAND,
    MODE_ARG
  } mode = MODE_COMMAND;
  char *toksave;

  spur.id = -1;
  lcoordinate = *coordinate;

  argv = strtok_r(line, " >\t\n\r", &toksave);
  if (!argv)
  {
    return 0;
  }

  while (argv)
  {
    switch (mode)
    {
      case MODE_COMMAND:
        for (i = 0; i < CS_MAX; i++)
        {
          if (strcmp(argv, YPSpur_CSName[i]) == 0)
          {
            lcoordinate = i;
            break;
          }
        }
        if (i != CS_MAX)
        {
          break;
        }
        for (i = 0; i < SPUR_COMMAND_MAX; i++)
        {
          if (strcmp(argv, SPUR_COMMAND[i].name) == 0)
          {
            spur = SPUR_COMMAND[i];
            break;
          }
        }
        mode = MODE_ARG;
        break;
      default:
        spur.arg[mode - MODE_ARG] = atof(argv);
        mode++;
        spur.narg = mode - MODE_ARG;
        if (mode > MODE_ARG + 3)
          break;
        break;
    }
    argv = strtok_r(NULL, " >\t\n\r", &toksave);
  }

  if (spur.id < 0)
  {
    *coordinate = lcoordinate;
    return 1;
  }

  if (spur.narg < spur.reqarg)
  {
    fprintf(stderr, "error: too few arguments to %s\n", spur.name);
    fflush(stderr);
    return 0;
  }

  switch (spur.id)
  {
    case SPUR_LINE:
      YPSpur_line(lcoordinate, spur.arg[0], spur.arg[1], spur.arg[2]);
      break;
    case SPUR_STOP_LINE:
      YPSpur_stop_line(lcoordinate, spur.arg[0], spur.arg[1], spur.arg[2]);
      break;
    case SPUR_CIRCLE:
      YPSpur_circle(lcoordinate, spur.arg[0], spur.arg[1], spur.arg[2]);
      break;
    case SPUR_ORIENT:
      YPSpur_orient(lcoordinate, spur.arg[0]);
      break;
    case SPUR_SPIN:
      YPSpur_spin(lcoordinate, spur.arg[0]);
      break;
    case SPUR_GETPOS:
      YPSpur_get_pos(lcoordinate, &spur.arg[0], &spur.arg[1], &spur.arg[2]);
      printf("%f %f %f\n", spur.arg[0], spur.arg[1], spur.arg[2]);
      break;
    case SPUR_GETVEL:
      YPSpur_get_vel(&spur.arg[0], &spur.arg[1]);
      printf("%f %f\n", spur.arg[0], spur.arg[1]);
      break;
    case SPUR_GETFORCE:
      YPSpur_get_force(&spur.arg[0], &spur.arg[1]);
      printf("%f %f\n", spur.arg[0], spur.arg[1]);
      break;
    case SPUR_GET_WHEEL_TORQUE:
      YP_get_wheel_torque(&spur.arg[0], &spur.arg[1]);
      printf("%f %f\n", spur.arg[0], spur.arg[1]);
      break;
    case SPUR_GET_VREF:
      YP_get_vref(&spur.arg[0], &spur.arg[1]);
      printf("%f %f\n", spur.arg[0], spur.arg[1]);
      break;
    case SPUR_GET_WHEEL_VREF:
      YP_get_wheel_vref(&spur.arg[0], &spur.arg[1]);
      printf("%f %f\n", spur.arg[0], spur.arg[1]);
      break;
    case SPUR_SETPOS:
      YPSpur_set_pos(lcoordinate, spur.arg[0], spur.arg[1], spur.arg[2]);
      break;
    case SPUR_NEAR_POS:
      ret = YPSpur_near_pos(lcoordinate, spur.arg[0], spur.arg[1], spur.arg[2]);
      printf("%d\n", ret);
      break;
    case SPUR_NEAR_ANG:
      ret = YPSpur_near_ang(lcoordinate, spur.arg[0], spur.arg[1]);
      printf("%d\n", ret);
      break;
    case SPUR_OVER_LINE:
      ret = YPSpur_over_line(lcoordinate, spur.arg[0], spur.arg[1], spur.arg[2]);
      printf("%d\n", ret);
      break;
    case SPUR_ADJUSTPOS:
      YPSpur_adjust_pos(lcoordinate, spur.arg[0], spur.arg[1], spur.arg[2]);
      break;
    case SPUR_SETVEL:
      YPSpur_set_vel(spur.arg[0]);
      break;
    case SPUR_SETANGVEL:
      YPSpur_set_angvel(spur.arg[0]);
      break;
    case SPUR_SETACCEL:
      YPSpur_set_accel(spur.arg[0]);
      break;
    case SPUR_SETANGACCEL:
      YPSpur_set_angaccel(spur.arg[0]);
      break;
    case SPUR_STOP:
      YPSpur_stop();
      break;
    case SPUR_INIT:
      break;
    case SPUR_FREE:
      YPSpur_free();
      break;
    case SPUR_OPENFREE:
      YP_openfree();
      break;
    case SPUR_FREEZE:
      YPSpur_freeze();
      break;
    case SPUR_UNFREEZE:
      YPSpur_unfreeze();
      break;
    case SPUR_ISFREEZE:
      ret = YPSpur_isfreeze();
      printf("%d\n", ret);
      break;
    case SPUR_GETAD:
      ad = YP_get_ad_value((int)spur.arg[0]);
      printf("%d\n", ad);
      break;
    case SPUR_SET_IO_DIR:
      YP_set_io_dir((int)spur.arg[0]);
      break;
    case SPUR_SET_IO_DATA:
      YP_set_io_data((int)spur.arg[0]);
      break;
    case SPUR_GET_DEVICE_ERROR_STATE:
      YP_get_device_error_state((int)spur.arg[0], &error);
      printf("%d\n", error);
      break;
    case SPUR_VEL:
      YPSpur_vel(spur.arg[0], spur.arg[1]);
      break;
    case SPUR_WHEEL_VEL:
      YP_wheel_vel(spur.arg[0], spur.arg[1]);
      break;
    case SPUR_WHEEL_TORQUE:
      YP_wheel_torque(spur.arg[0], spur.arg[1]);
      break;
    case SPUR_GET_WHEEL_VEL:
      YP_get_wheel_vel(&spur.arg[0], &spur.arg[1]);
      printf("%f %f\n", spur.arg[0], spur.arg[1]);
      break;
    case SPUR_GET_WHEEL_ANG:
      YP_get_wheel_ang(&spur.arg[0], &spur.arg[1]);
      printf("%f %f\n", spur.arg[0], spur.arg[1]);
      break;
    case SPUR_SET_WHEEL_VEL:
      YP_set_wheel_vel(spur.arg[0], spur.arg[1]);
      break;
    case SPUR_SET_WHEEL_ACCEL:
      YP_set_wheel_accel(spur.arg[0], spur.arg[1]);
      break;
    case SPUR_WHEEL_ANG:
      YP_wheel_ang(spur.arg[0], spur.arg[1]);
      break;
    case SPUR_JOINT_TORQUE:
      YP_joint_torque((int)spur.arg[0], spur.arg[1]);
      break;
    case SPUR_JOINT_VEL:
      YP_joint_vel((int)spur.arg[0], spur.arg[1]);
      break;
    case SPUR_JOINT_ANG:
      YP_joint_ang((int)spur.arg[0], spur.arg[1]);
      break;
    case SPUR_JOINT_ANG_VEL:
      YP_joint_ang_vel((int)spur.arg[0], spur.arg[1], spur.arg[2]);
      break;
    case SPUR_SET_JOINT_ACCEL:
      YP_set_joint_accel((int)spur.arg[0], spur.arg[1]);
      break;
    case SPUR_SET_JOINT_VEL:
      YP_set_joint_vel((int)spur.arg[0], spur.arg[1]);
      break;
    case SPUR_GET_JOINT_VEL:
      YP_get_joint_vel((int)spur.arg[0], &spur.arg[1]);
      printf("%f\n", spur.arg[1]);
      break;
    case SPUR_GET_JOINT_VREF:
      YP_get_joint_vref((int)spur.arg[0], &spur.arg[1]);
      printf("%f\n", spur.arg[1]);
      break;
    case SPUR_GET_JOINT_ANG:
      YP_get_joint_ang((int)spur.arg[0], &spur.arg[1]);
      printf("%f\n", spur.arg[1]);
      break;
    case SPUR_GET_JOINT_TORQUE:
      YP_get_joint_torque((int)spur.arg[0], &spur.arg[1]);
      printf("%f\n", spur.arg[1]);
      break;
    case SPUR_SLEEP:
      yp_usleep((int)(spur.arg[0] * 1000.0 * 1000.0));
      break;
    case SPUR_REQUEST_DEVICE_DUMP:
      YP_request_device_dump((int)spur.arg[0], (int)spur.arg[1]);
      break;
    case HELP:
      printf("Usage:\n");
      printf("   > command arg1 arg2 arg3\n");
      printf("   > coordinate\n");
      printf("   > coordinate command arg1 arg2 arg3\n");
      printf("   > coordinate> command arg1 arg2 arg3\n");
      printf("Commands:\n");
      for (i = 0; i < SPUR_COMMAND_MAX; i++)
      {
        printf("   %s\n", SPUR_COMMAND[i].name);
      }
      printf("Coordinates:\n");
      for (i = 0; i < CS_MAX + 1; i++)
      {
        printf("   %s\n", YPSpur_CSName[i]);
      }
      break;
    case EXIT:
      return -1;
      break;
    default:
      printf("unknown command\n");
  }
  fflush(stdout);

  return 1;
}

int proc_spur(char *line, int *coordinate)
{
  char *line_div;
  char *toksave;
  int ret;

  ret = 0;
  line_div = strtok_r(line, ";\n\r", &toksave);
  while (line_div)
  {
    ret = proc_spur_cmd(line_div, coordinate);
    if (ret < 0)
    {
      break;
    }
    line_div = strtok_r(NULL, ";\n\r", &toksave);
  }
  return ret;
}

void print_help(char *argv[])
{
  fprintf(stderr, "USAGE: %s\n", argv[0]);
  fputs("\t-V | --set-vel      VALUE    : [m/s]   set max vel of SPUR to VALUE\n", stderr);
  fputs("\t-W | --set-angvel   VALUE    : [rad/s] set max angvel of SPUR to VALUE\n", stderr);
  fputs("\t-A | --set-accel    VALUE    : [m/ss]   set accel of SPUR to VALUE\n", stderr);
  fputs("\t-O | --set-angaccel VALUE    : [rad/ss] set angaccel of SPUR to VALUE\n", stderr);
  fputs("\t-c | --command     \"VALUE\"   : execute command VALUE\n", stderr);
  fputs("\t-q | --msq-id       VALUE    : set message-queue id\n", stderr);
  fputs("\t-s | --socket       IP:PORT  : use socket ipc\n", stderr);
  fputs("\t-h | --help                  : print this help\n", stderr);
}

int main(int argc, char *argv[])
{
  int coordinate = CS_FS;
  char ip[64];
  int port = 0;
  int active = 1;
  int err = 0;
  double vel = 0;
  double angvel = 0;
  double accel = 0;
  double angaccel = 0;
  int set_vel = 0;
  int set_accel = 0;
  int set_angvel = 0;
  int set_angaccel = 0;
  int msqid = 0;
  struct option options[9] =
      {
        { "set-vel", 1, 0, 'V' },
        { "set-angvel", 1, 0, 'W' },
        { "set-accel", 1, 0, 'A' },
        { "set-angaccel", 1, 0, 'O' },
        { "command", 1, 0, 'c' },
        { "msq-id", 1, 0, 'q' },
        { "socket", 1, 0, 's' },
        { "help", 0, 0, 'h' },
        { 0, 0, 0, 0 }
      };
  int opt;

  hook_pre_global();

  while ((opt = getopt_long(argc, argv, "V:W:A:O:c:q:s:h", options, NULL)) != -1)
  {
    switch (opt)
    {
      case 'V':
        vel = atof(optarg);
        set_vel = 1;
        break;
      case 'W':
        angvel = atof(optarg);
        set_angvel = 1;
        break;
      case 'A':
        accel = atof(optarg);
        set_accel = 1;
        break;
      case 'O':
        angaccel = atof(optarg);
        set_angaccel = 1;
        break;
      case 'c':
        if (msqid == -1)
          YPSpur_init_socket(ip, port);
        else if (msqid == 0)
          YPSpur_init();
        else
          YPSpur_initex(msqid);
        proc_spur(optarg, &coordinate);
        return 1;
        break;
      case 'q':
        msqid = atoi(optarg);
        break;
      case 's':
        strncpy(ip, optarg, 64);
        {
          char *p;
          p = strchr(ip, ':');
          if (p == NULL)
          {
            fprintf(stderr, "USAGE: %s -s ip:port\n", argv[0]);
            return -1;
          }
          *p = 0;
          port = atoi(p + 1);
          msqid = -1;
        }
        break;
      case 'h':
        print_help(argv);
        return 1;
        break;
      default:
        break;
    }
  }

#if HAVE_SIGLONGJMP
  signal(SIGINT, ctrlc);
#endif  // HAVE_SIGLONGJMP

  if (msqid == -1)
    YPSpur_init_socket(ip, port);
  else if (msqid == 0)
    YPSpur_init();
  else
    YPSpur_initex(msqid);

  if (set_vel)
    YPSpur_set_vel(vel);
  if (set_angvel)
    YPSpur_set_angvel(angvel);
  if (set_accel)
    YPSpur_set_accel(accel);
  if (set_angaccel)
    YPSpur_set_angaccel(angaccel);

#if HAVE_LIBREADLINE
  using_history();
  read_history(".spurip_history");
#endif  // HAVE_LIBREADLINE
  while (active)
  {
    static char *line = NULL;
    static char *line_prev = NULL;
    char text[16];
#if !HAVE_LIBREADLINE
#if HAVE_GETLINE
    size_t len;
#endif  // HAVE_GETLINE
#endif  // !HAVE_LIBREADLINE
#if HAVE_SIGLONGJMP
    if (sigsetjmp(ctrlc_capture, 1) != 0)
    {
#if HAVE_LIBREADLINE
      write_history(".spurip_history");
#endif  // HAVE_LIBREADLINE
    }
    else
#endif  // HAVE_SIGLONGJMP
    {
      {
        // Dummy for error checking
        double x, y, th;
        YPSpur_get_pos(CS_GL, &x, &y, &th);
      }
      if (YP_get_error_state())
      {
        if (msqid == -1)
          YPSpur_init_socket(ip, port);
        else if (msqid == 0)
          YPSpur_init();
        else
          YPSpur_initex(msqid);
        if (set_vel)
          YPSpur_set_vel(vel);
        if (set_angvel)
          YPSpur_set_angvel(angvel);
        if (set_accel)
          YPSpur_set_accel(accel);
        if (set_angaccel)
          YPSpur_set_angaccel(angaccel);
        if (err == 0)
        {
          fprintf(stderr, "WARN: YPSpur-coordinator terminated.\n");
          fflush(stderr);
#if HAVE_SIGLONGJMP
          signal(SIGINT, NULL);
#endif  // HAVE_SIGLONGJMP
        }
        err = 1;
        yp_usleep(50000);
        continue;
      }
      else
      {
        if (err == 1)
        {
          fprintf(stderr, "INFO: YPSpur-coordinator started.\n");
          fflush(stderr);
#if HAVE_SIGLONGJMP
          signal(SIGINT, ctrlc);
#endif  // HAVE_SIGLONGJMP
        }
      }
      err = 0;

      snprintf(text, sizeof(text), "%s> ", YPSpur_CSName[coordinate]);
#if HAVE_LIBREADLINE
      line_prev = line;
      line = readline(text);
      if (!line)
      {
        // EOF
        break;
      }
      if (strlen(line) > 0)
      {
        if (line && line_prev)
        {
          if (strcmp(line, line_prev) != 0)
          {
            add_history(line);
          }
        }
        else
        {
          add_history(line);
        }
      }
#else
      printf("%s", text);
      fflush(stdout);
      line = NULL;
#if HAVE_GETLINE
      len = 0;
      getline(&line, &len, stdin);
      if (len == 0)
        continue;
#else
      line = malloc(512);
      fgets(line, 512, stdin);
#endif  // HAVE_GETLINE
      line_prev = line;
#endif  // HAVE_LIBREADLINE
      if (proc_spur(line, &coordinate) < 0)
      {
        active = 0;
      }

      if (line_prev)
        free(line_prev);
    }
  }
  printf("\n");
#if HAVE_LIBREADLINE
  write_history(".spurip_history");
#endif  // HAVE_LIBREADLINE

  return 0;
}
