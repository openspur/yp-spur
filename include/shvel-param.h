#ifndef __PARAMETER__
#define __PARAMETER__

typedef union
{
	int integer;
	char byte[4];
} Int_4Char;

typedef union _short_2char
{
	short integer;
	char byte[2];
} Short_2Char;

// typedef 
enum
{
	SERVO_LEVEL_STOP = 0,
	SERVO_LEVEL_COUNTER,
	SERVO_LEVEL_TORQUE,
	SERVO_LEVEL_VELOCITY,
	SERVO_LEVEL_POSITION, // not used
	SERVO_LEVEL_OPENFREE,
};

typedef enum
{
	PARAM_w_ref = 0,
	PARAM_w_ref_highprec,
	PARAM_p_ki,
	PARAM_p_kv,
	PARAM_p_fr_plus,
	PARAM_p_fr_wplus,
	PARAM_p_fr_minus,
	PARAM_p_fr_wminus,
	PARAM_p_A,
	PARAM_p_B,
	PARAM_p_C,
	PARAM_p_D,
	PARAM_p_E,
	PARAM_p_F,
	PARAM_p_pi_kp,
	PARAM_p_pi_ki,
	PARAM_pwm_max,
	PARAM_pwm_min,
	PARAM_toq_max,
	PARAM_toq_min,
	PARAM_int_max,
	PARAM_int_min,
	PARAM_p_toq_offset,
	PARAM_toq_limit,
	PARAM_enc_rev,
	PARAM_motor_phase,
	PARAM_vsrc,
	PARAM_p_inertia_self,
	PARAM_p_inertia_cross,
	PARAM_enc_type,
	PARAM_control_cycle,
	PARAM_enc_div,
	PARAM_servo = 64,
	PARAM_watch_dog_limit,
	PARAM_io_dir = 96,
	PARAM_io_data,
	PARAM_ad_mask,
	PARAM_phase_offset,
} YPSpur_shvel_param;

typedef enum
{
	INT_enc_index_rise = 0,
	INT_enc_index_fall,
} YPSpur_shvel_interrupt;

#endif
