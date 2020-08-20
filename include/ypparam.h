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

#ifndef YPPARAM_H
#define YPPARAM_H

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#if defined(_WIN32)
#if !defined(_WIN64)
typedef int pid_t;
#endif  // !defined(_WIN64)
#else
#include <unistd.h>
#endif  // defined(_WIN32)

/* コマンドナンバー */
enum
{
  YPSPUR_FREE = 0x10,
  YPSPUR_SERVO,
  YPSPUR_OPENFREE,

  YPSPUR_VEL = 0x20,
  YPSPUR_LINE,
  YPSPUR_CIRCLE,
  YPSPUR_WHEEL_VEL,
  YPSPUR_WHEEL_TORQUE,

  YPSPUR_ORIENT = 0x30,

  YPSPUR_SPIN = 0x40,
  YPSPUR_STOP_LINE,
  YPSPUR_STOP,
  YPSPUR_WHEEL_ANGLE,

  YPSPUR_SET_VEL = 0x50,
  YPSPUR_SET_ANGVEL,
  YPSPUR_SET_ACCEL,
  YPSPUR_SET_ANGACCEL,
  YPSPUR_SET_POS,
  YPSPUR_SET_GL_GL,
  YPSPUR_SET_TILT,
  YPSPUR_ADJUST,
  YPSPUR_SET_WHEEL_VEL,
  YPSPUR_SET_WHEEL_ACCEL,

  YPSPUR_GET_POS = 0x70,
  YPSPUR_GET_VEL,
  YPSPUR_NEAR_POS,
  YPSPUR_NEAR_ANG,
  YPSPUR_OVER_LINE,
  YPSPUR_GET_WHEEL_VEL,
  YPSPUR_GET_WHEEL_ANG,
  YPSPUR_GET_FORCE,
  YPSPUR_GET_VREF,
  YPSPUR_GET_WHEEL_VREF,
  //
  YPSPUR_PARAM_SET = 0x100,
  YPSPUR_PARAM_GET,
  YPSPUR_PARAM_STATE,
  YPSPUR_GET_WHEEL_TORQUE,

  YPSPUR_FREEZE = 0x200,
  YPSPUR_UNFREEZE,
  YPSPUR_ISFREEZE,
  // 拡張
  YPSPUR_GETAD = 0x500,
  YPSPUR_SETIODIR,
  YPSPUR_SETIODATA,
  YPSPUR_GET_ERROR_STATE,
  YPSPUR_REQUEST_DEVICE_DUMP,

  YPSPUR_JOINT_TORQUE = 0x800,
  YPSPUR_JOINT_VEL,
  YPSPUR_JOINT_ANG,
  YPSPUR_JOINT_ANG_VEL,

  YPSPUR_SET_JOINT_ACCEL = 0x810,
  YPSPUR_SET_JOINT_VEL,

  YPSPUR_GET_JOINT_VEL = 0x820,
  YPSPUR_GET_JOINT_VREF,
  YPSPUR_GET_JOINT_ANG,
  YPSPUR_GET_JOINT_TORQUE,
};

/* パラメータナンバー */
/* set_parameter用 */
/* マイコンに送る情報は反映されない */
typedef enum
{
  // システム
  YP_PARAM_VERSION = 0,

  YP_PARAM_TORQUE_UNIT,
  YP_PARAM_TORQUE_FINENESS,
  YP_PARAM_PWM_MAX,

  // モータパラメータ
  YP_PARAM_COUNT_REV,
  YP_PARAM_ENCODER_TYPE,
  YP_PARAM_VOLT,
  YP_PARAM_CYCLE,
  YP_PARAM_GEAR,
  YP_PARAM_MOTOR_R,
  YP_PARAM_MOTOR_TC,
  YP_PARAM_MOTOR_VC,
  YP_PARAM_MOTOR_PHASE,
  YP_PARAM_PHASE_OFFSET,

  // キネマティクス
  YP_PARAM_RADIUS,
  YP_PARAM_RADIUS_R,  // 後方互換のため
  YP_PARAM_RADIUS_L,  // 後方互換のため
  YP_PARAM_TREAD,

  // 車体コントロールパラメータ
  YP_PARAM_CONTROL_CYCLE,
  YP_PARAM_MAX_VEL,
  YP_PARAM_MAX_W,
  YP_PARAM_MAX_ACC_V,
  YP_PARAM_MAX_ACC_W,
  YP_PARAM_MAX_CENTRIFUGAL_ACC,

  // Spur軌跡追従パラメータ
  YP_PARAM_L_C1,
  YP_PARAM_L_K1,
  YP_PARAM_L_K2,
  YP_PARAM_L_K3,
  YP_PARAM_L_DIST,

  // モータ制御パラメータ
  YP_PARAM_GAIN_KP,
  YP_PARAM_GAIN_KI,
  YP_PARAM_TORQUE_MAX,
  YP_PARAM_TORQUE_NEWTON,
  YP_PARAM_TORQUE_VISCOS,
  YP_PARAM_TORQUE_NEWTON_NEG,
  YP_PARAM_TORQUE_VISCOS_NEG,
  YP_PARAM_INTEGRAL_MAX,
  YP_PARAM_TORQUE_OFFSET,
  YP_PARAM_TORQUE_LIMIT,

  // 慣性パラメータ
  YP_PARAM_MASS,
  YP_PARAM_MOMENT_INERTIA,
  YP_PARAM_MOTOR_M_INERTIA,
  YP_PARAM_TIRE_M_INERTIA,

  // ボディサイズパラメータ
  YP_PARAM_SIZE_FRONT,
  YP_PARAM_SIZE_REAR,
  YP_PARAM_SIZE_LEFT,
  YP_PARAM_SIZE_RIGHT,

  // 慣性・イナーシャ補償パラメータ(上の情報を使って自動計算)
  YP_PARAM_GAIN_A,
  YP_PARAM_GAIN_B,
  YP_PARAM_GAIN_C,
  YP_PARAM_GAIN_D,
  YP_PARAM_GAIN_E,
  YP_PARAM_GAIN_F,

  // サーボ制御パラメータ
  YP_PARAM_STOP_LINEAR,
  YP_PARAM_SPIN_LINEAR,
  YP_PARAM_WHEEL_ANG_LINEAR,

  YP_PARAM_MIN_WHEEL_ANGLE,
  YP_PARAM_MAX_WHEEL_ANGLE,

  YP_PARAM_VEHICLE_CONTROL,
  YP_PARAM_CONTROL_MODE_RESEND,

  // イナーシャパラメータ
  YP_PARAM_INERTIA_SELF,
  YP_PARAM_INERTIA_CROSS,

  // エンコーダ拡張
  YP_PARAM_ENCODER_DIV,
  YP_PARAM_ENCODER_DENOMINATOR,
  YP_PARAM_HALL_DELAY,
  YP_PARAM_LR_CUTOFF_FREQ,

  // 絶対角
  YP_PARAM_INDEX_RISE_ANGLE,
  YP_PARAM_INDEX_FALL_ANGLE,
  YP_PARAM_INDEX_GEAR,

  YP_PARAM_DEVICE_TIMEOUT,

  YP_PARAM_NUM  ///< パラメータの最大値
} YPSpur_param;

// 自分で指定しない（できない）パラメータは最初の文字が'_'で始まっている
#define YP_PARAM_NAME          \
  {                            \
    "VERSION",                 \
        "_TORQUE_UNIT",        \
        "TORQUE_FINENESS",     \
        "_PWM_RESOLUTION",     \
        "COUNT_REV",           \
        "ENCODER_TYPE",        \
        "VOLT",                \
        "CYCLE",               \
        "GEAR",                \
        "MOTOR_R",             \
        "MOTOR_TC",            \
        "MOTOR_VC",            \
        "MOTOR_PHASE",         \
        "PHASE_OFFSET",        \
        "RADIUS",              \
        "RADIUS_R",            \
        "RADIUS_L",            \
        "TREAD",               \
        "CONTROL_CYCLE",       \
        "MAX_VEL",             \
        "MAX_W",               \
        "MAX_ACC_V",           \
        "MAX_ACC_W",           \
        "MAX_CENTRI_ACC",      \
        "L_C1",                \
        "L_K1",                \
        "L_K2",                \
        "L_K3",                \
        "L_DIST",              \
        "GAIN_KP",             \
        "GAIN_KI",             \
        "TORQUE_MAX",          \
        "TORQUE_NEWTON",       \
        "TORQUE_VISCOS",       \
        "-TORQUE_NEWTON",      \
        "-TORQUE_VISCOS",      \
        "INTEGRAL_MAX",        \
        "TORQUE_OFFSET",       \
        "TORQUE_LIMIT",        \
        "MASS",                \
        "MOMENT_INERTIA",      \
        "MOTOR_M_INERTIA",     \
        "TIRE_M_INERTIA",      \
        "SIZE_FRONT",          \
        "SIZE_REAR",           \
        "SIZE_LEFT",           \
        "SIZE_RIGHT",          \
        "_GAIN_A",             \
        "_GAIN_B",             \
        "_GAIN_C",             \
        "_GAIN_D",             \
        "_GAIN_E",             \
        "_GAIN_F",             \
        "STOP_LINEAR",         \
        "SPIN_LINEAR",         \
        "WHEEL_ANG_LINEAR",    \
        "MIN_WHEEL_ANGLE",     \
        "MAX_WHEEL_ANGLE",     \
        "VEHICLE_CONTROL",     \
        "CONTROL_MODE_RESEND", \
        "_INERTIA_SELF",       \
        "_INERTIA_CROSS",      \
        "ENCODER_DIV",         \
        "ENCODER_DENOMINATOR", \
        "HALL_DELAY",          \
        "LR_CUTOFF_FREQ",      \
        "INDEX_RISE_ANGLE",    \
        "INDEX_FALL_ANGLE",    \
        "INDEX_GEAR",          \
        "DEVICE_TIMEOUT",      \
  }

#define YP_PARAM_NECESSARY \
  {                        \
    1,                     \
        0,                 \
        1,                 \
        0,                 \
        1,                 \
        0,                 \
        1,                 \
        1,                 \
        1,                 \
        1,                 \
        1,                 \
        1,                 \
        0,                 \
        0,                 \
        1,                 \
        0,                 \
        0,                 \
        1,                 \
        1,                 \
        1,                 \
        1,                 \
        1,                 \
        1,                 \
        1,                 \
        1,                 \
        1,                 \
        1,                 \
        1,                 \
        1,                 \
        1,                 \
        1,                 \
        1,                 \
        0,                 \
        0,                 \
        0,                 \
        0,                 \
        1,                 \
        0,                 \
        0,                 \
        1,                 \
        1,                 \
        1,                 \
        1,                 \
        0,                 \
        0,                 \
        0,                 \
        0,                 \
        0,                 \
        0,                 \
        0,                 \
        0,                 \
        0,                 \
        0,                 \
        0,                 \
        0,                 \
        0,                 \
        0,                 \
        0,                 \
        0,                 \
        0,                 \
        0,                 \
        0,                 \
        0,                 \
        0,                 \
        0,                 \
        0,                 \
        0,                 \
        0,                 \
        0,                 \
        0,                 \
  }

#define YP_PARAM_COMMENT                                                                        \
  {                                                                                             \
    "Parameter file version",                                                                   \
        "[Integer Nm/Nm] Fixed-point position of PC-MCU communication",                         \
        "[Nm] Calculation fineness of torque control",                                          \
        "[Counts] PWM cycle",                                                                   \
        "[Counts/rev] Encoder specification",                                                   \
        "Encoder type (2:2-phase incremental, 3:3-phase incremental)",                          \
        "[V] Power source voltage",                                                             \
        "[s] Velocity control cycle",                                                           \
        "[in/out] Gear ratio",                                                                  \
        "[ohm] Motor internal resistance",                                                      \
        "[Nm/A] Motor torque constant",                                                         \
        "[rpm/V] Motor speed constant",                                                         \
        "Motor type (0:DC, 3:3phase-AC)",                                                       \
        "[rad] Offset angle of AC motor phase",                                                 \
        "[m] Wheel radius",                                                                     \
        "[m] Right wheel radius",                                                               \
        "[m] Left wheel radius",                                                                \
        "[m] Tread",                                                                            \
        "[s] Trajectory control cycle",                                                         \
        "[m/s] Maximum velocity",                                                               \
        "[rad/s] Maximum angular velocity",                                                     \
        "[m/ss] Maximum acceleration",                                                          \
        "[rad/ss] Maximum angular acceleration",                                                \
        "[m/ss] Centrifugal acceleration limit",                                                \
        "[m/s / rad/s] Deacceleration factor of trajectory control",                            \
        "[rad/ss / m] Feedback gain for distance error",                                        \
        "[rad/ss / rad] Feedback gain for angular error",                                       \
        "[rad/ss / rad/s] Feedback gain for angular velocity",                                  \
        "[m] Clipping value of line following control",                                         \
        "[1/s] PI control parameter Kp",                                                        \
        "[1/ss] PI control parameter Ki",                                                       \
        "[Nm] Motor maximum torque",                                                            \
        "[Nm] Constant friction",                                                               \
        "[Nm/(rad/s)] Viscous friction",                                                        \
        "[Nm] Constant friction (neg-direction)",                                               \
        "[Nm/(rad/s)] Viscous friction (neg-direction)",                                        \
        "[Nm] Motor torque limit",                                                              \
        "[rev] Maximum integrated error of PI control",                                         \
        "[Nm] Offset value of motor torque",                                                    \
        "[kg] Robot weight",                                                                    \
        "[kgm^2] Robot moment of inertia",                                                      \
        "[kgm^2] Rotor moment of inertia of motor",                                             \
        "[kgm^2] Tire moment of inertia",                                                       \
        "[m] Robot size of front",                                                              \
        "[m] Robot size of rear",                                                               \
        "[m] Robot size of left",                                                               \
        "[m] Robot size of right",                                                              \
        "PWS parameter A",                                                                      \
        "PWS parameter B",                                                                      \
        "PWS parameter C",                                                                      \
        "PWS parameter D",                                                                      \
        "PWS parameter E",                                                                      \
        "PWS parameter F",                                                                      \
        "[m] Linear feedback area of stop command",                                             \
        "[rad] Linear feedback area of spin command",                                           \
        "[rad] Linear feedback area of wheel_ang command",                                      \
        "[rad] Minimum wheel angle (for wheel_angle command)",                                  \
        "[rad] Maximum wheel angle (for wheel_angle command)",                                  \
        "Used for vehicle control (0: false, 1: true)",                                         \
        "[s] Time-span to resend control mode (0: don't resend)",                               \
        "Motor load inertia",                                                                   \
        "Motor load cross inertia",                                                             \
        "Encoder count divider 2^x (e.g. 4 means divide by 16)",                                \
        "Encoder count denominator"                                                             \
        " (COUNT_REV/ENCODER_DENOMINATOR is encoder resolution for one electrical revolution)", \
        "[s] Hall signal delay",                                                                \
        "[Hz] Motor LR cutoff frequency",                                                       \
        "[rad] Index signal rising edge angle at CW rotation",                                  \
        "[rad] Index signal falling edge angle at CW rotation",                                 \
        "[in/out] Index signal gear ratio",                                                     \
        "[s] Timeout of the communication with the device",                                     \
  }

enum motor_id
{
  MOTOR_RIGHT = 0,
  MOTOR_LEFT,
};
#define YP_PARAM_MAX_MOTOR_NUM 16

#define YP_PARAM_ALIAS_NUM 2
// clang-format off
#define YP_PARAM_ALIAS                                 \
  {                                                    \
    {                                                  \
      YP_PARAM_RADIUS_L, YP_PARAM_RADIUS, MOTOR_LEFT   \
    },                                                 \
    {                                                  \
      YP_PARAM_RADIUS_R, YP_PARAM_RADIUS, MOTOR_RIGHT  \
    }                                                  \
  }
// clang-format on

#define YP_PARAM_REQUIRED_VERSION 4.0
#define YP_PARAM_SUPPORTED_VERSION 5.0

/* パラメータの有効・無効 */
/* control_state用　　 */
typedef enum
{
  YP_STATE_MOTOR = 0,
  YP_STATE_VELOCITY,
  YP_STATE_BODY,
  YP_STATE_TRACKING,
  YP_STATE_GRAVITY,

  YP_STATE_NUM
} YPSpur_state;

/** メッセージ関連 */
typedef enum
{
  CS_BS = 0,
  CS_SP,
  CS_GL,
  CS_LC,
  CS_FS,
  CS_BL,
  CS_MAX
} YPSpur_cs;

static const char YPSpur_CSName[CS_MAX][16] = {
  { "BS" },
  { "SP" },
  { "GL" },
  { "LC" },
  { "FS" },
  { "BL" },
};

typedef struct
{
  long msg_type;
  long pid;
  int type;
  int cs;
  double data[4];
} YPSpur_msg;

#define YPSPUR_MAX_SOCKET 64

struct ipcmd_t
{
  enum ipcmd_type_t
  {
    IPCMD_MSQ,
    IPCMD_TCP
  } type;
  enum ipcmd_tcp_type_t
  {
    IPCMD_TCP_SERVER,
    IPCMD_TCP_CLIENT
  } tcp_type;
  int socket;
  int clients[YPSPUR_MAX_SOCKET];
  int connection_error;
  pid_t pid;
  pid_t pids[YPSPUR_MAX_SOCKET];
  int (*send)(struct ipcmd_t *ipcmd, YPSpur_msg *data);
  int (*recv)(struct ipcmd_t *ipcmd, YPSpur_msg *data);
  void (*flush)(struct ipcmd_t *ipcmd);
};

#define YPSPUR_MSQ_KEY 0x7045
#define YPSPUR_MSG_CMD 1
#define YPSPUR_MSG_SIZE (sizeof(YPSpur_msg) - sizeof(long))

#define ENABLE 1
#define DISABLE 0

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // YPPARAM_H
