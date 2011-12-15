#ifndef __YP_PARAM_H__
#define __YP_PARAM_H__

#ifdef __cplusplus
extern "C"
{
#endif											// __cplusplus

	/* コマンドナンバー */
	enum
	{
		YPSPUR_FREE = 0x10,
		YPSPUR_SERVO,

		YPSPUR_VEL = 0x20,
		YPSPUR_WHEEL_VEL,
		YPSPUR_LINE,
		YPSPUR_CIRCLE,

		YPSPUR_ORIENT = 0x30,

		YPSPUR_SPIN = 0x40,
		YPSPUR_STOP_LINE,
		YPSPUR_STOP,

		YPSPUR_SET_VEL = 0x50,
		YPSPUR_SET_ANGVEL,
		YPSPUR_SET_ACCEL,
		YPSPUR_SET_ANGACCEL,
		YPSPUR_SET_POS,
		YPSPUR_SET_GL_GL,
		YPSPUR_SET_TILT,
		YPSPUR_ADJUST,

		YPSPUR_GET_POS = 0x70,
		YPSPUR_GET_VEL,
		YPSPUR_NEAR_POS,
		YPSPUR_NEAR_ANG,
		YPSPUR_OVER_LINE,
		YPSPUR_GET_WHEEL_VEL,
		YPSPUR_GET_WHEEL_ANG,
		YPSPUR_GET_FORCE,
		// 
		YPSPUR_PARAM_SET = 0x100,
		YPSPUR_PARAM_GET,
		YPSPUR_PARAM_STATE,
		YPSPUR_WHEEL_TORQUE,
		YPSPUR_GET_WHEEL_TORQUE,

		YPSPUR_FREEZE = 0x200,
		YPSPUR_UNFREEZE,
		YPSPUR_ISFREEZE,
		// 拡張
		YPSPUR_GETAD = 0x500,
		YPSPUR_SETIODIR,
		YPSPUR_SETIODATA,
	};

	/* パラメータナンバー */
	/* set_parameter用 */
	/* マイコンに送る情報は反映されない */
	typedef enum
	{
		// システム
		YP_PARAM_VERSION = 0,

		YP_PARAM_TORQUE_UNIT,
		YP_PARAM_VOLT_UNIT,
		YP_PARAM_METER_UNIT,
		YP_PARAM_ANPERE_UNIT,
		YP_PARAM_AVEL_UNIT,
		YP_PARAM_PWM_MAX,

		// モータパラメータ
		YP_PARAM_COUNT_REV,
		YP_PARAM_VOLT,
		YP_PARAM_CYCLE,
		YP_PARAM_GEAR,
		YP_PARAM_MOTOR_R,
		YP_PARAM_MOTOR_TC,
		YP_PARAM_MOTOR_VC,
		YP_PARAM_MOTOR_VTC,				///< 未使用（用途不明）

		// キネマティクス
		YP_PARAM_RADIUS_R,
		YP_PARAM_RADIUS_L,
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
		YP_PARAM_INTEGRAL_MAX,
		YP_PARAM_TORQUE_OFFSET,

		// 慣性パラメータ
		YP_PARAM_MASS,							///< [kg] ロボット全体の質量
		YP_PARAM_MOMENT_INERTIA,				///< [kgm^2] ロボット全体の慣性モーメント
		YP_PARAM_MOTOR_M_INERTIA,				///< [kgm^2] モーターの慣性モーメント
		YP_PARAM_TIRE_M_INERTIA,				///< [kgm^2] タイヤのイナーシャ

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

		YP_PARAM_NUM							///< パラメータの最大値
	} YPSpur_param;


// 自分で指定しない（できない）パラメータは最初の文字が'_'で始まっている
#define YP_PARAM_NAME \
{\
	"VERSION", "TORQUE_UNIT","VOLT_UNIT", "METER_UNIT", "ANPERE_UNIT", "AVEL_UNIT",\
	"PWM_MAX", "COUNT_REV", "VOLT", "CYCLE", "GEAR", "MOTOR_R", "MOTOR_TC", "MOTOR_VC", "_MOTOR_VTC",\
	"RADIUS_R", "RADIUS_L", "TREAD", "CONTROL_CYCLE",\
	"MAX_VEL", "MAX_W", "MAX_ACC_V", "MAX_ACC_W", "MAX_CENTRI_ACC",\
	"L_C1", "L_K1", "L_K2", "L_K3", "L_DIST", "GAIN_KP", "GAIN_KI",\
	"TORQUE_MAX", "TORQUE_NEWTON", "TORQUE_VISCOS", "INTEGRAL_MAX", "_TORQUE_OFFSET",\
	"MASS","MOMENT_INERTIA", "MOTOR_M_INERTIA", "TIRE_M_INERTIA",\
	"SIZE_FRONT", "SIZE_REAR", "SIZE_LEFT", "SIZE_RIGHT",\
	"_GAIN_A","_GAIN_B","_GAIN_C","_GAIN_D","_GAIN_E","_GAIN_F",\
}

#define YP_PARAM_COMMENT \
{\
	"","[Integer Nm/Nm]","[Integer V/V]", "[Integer m/m]", "[Integer A/A]","[Integer rad/s / rad/s]",\
	"(28000000/DUTY)","[Counts/rev]","[V]","[s]", "[in/out]","[ohm]","[Nm/A]","[rpm/V]","",\
	"[m]","[m]","[m]","[s]",\
	"[m/s]", "[rad/s]","[m/ss]","[rad/ss]","[m/ss] Centrifugal acceleration limit",\
	"","","","","Clipping value of line following control","[1/s]PI control parameter Kp","[1/ss]PI control parameter Ki",\
	"[Nm]","[Nm]","[Nm/(rad/s)]","[rev]","[Nm]",\
	"[kg]","[kgm^2]","[kgm^2]","[kgm^2]",\
	"[m]","[m]","[m]","[m]",\
	"PWS parameter A","PWS parameter B","PWS parameter C","PWS parameter D","PWS parameter E","PWS parameter F",\
}

#define YP_PARAM_REQUIRED_VERSION	3.0

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
		{"BS"},
		{"SP"},
		{"GL"},
		{"LC"},
		{"FS"},
		{"BL"},
	};

	typedef struct
	{
		long msg_type;
		long pid;
		int type;
		int cs;
		double data[4];
	} YPSpur_msg;

#define YPSPUR_MSQ_KEY 0x7045
#define YPSPUR_MSG_CMD 1
#define YPSPUR_MSG_SIZE (sizeof(YPSpur_msg) -sizeof(long))

#define ENABLE 1
#define DISABLE 0


#ifdef __cplusplus
}
#endif											// __cplusplus

#endif											// __YP_PARAM_H__
