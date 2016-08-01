#ifndef __PARAM__
#define __PARAM__

#include <ypparam.h>
#include <utility.h>

#include <pthread.h>

typedef enum
{
	OPTION_SHOW_ODOMETRY   = 0x0001,
	OPTION_PARAM_CONTROL   = 0x0002,
	OPTION_SHOW_TIMESTAMP  = 0x0004,
	OPTION_SHOW_HELP       = 0x0008,
	OPTION_SHOW_LONGHELP   = 0x0010,
	OPTION_SHOW_PARAMHELP  = 0x0020,
	OPTION_VERSION         = 0x0040,
	OPTION_DAEMON          = 0x0080,

	OPTION_SOCKET                = 0x00200,
	OPTION_PARAM_FILE            = 0x00400,

	OPTION_WITHOUT_DEVICE        = 0x00800,
	OPTION_WITHOUT_SSM           = 0x01000,
	OPTION_ENABLE_SET_BS         = 0x02000,
	OPTION_DO_NOT_USE_YP         = 0x04000,
	OPTION_RECONNECT             = 0x08000,
	OPTION_ENABLE_GET_DIGITAL_IO = 0x10000,
	OPTION_PASSIVE               = 0x20000,
	OPTION_UPDATE_PARAM          = 0x40000,
	OPTION_HIGH_PREC             = 0x80000,
} ParamOptions;

#define OPTION_DEFAULT (OPTION_HIGH_PREC)

#define DEFAULT_PARAMETER_FILE "./robot.param"
#define DEFAULT_DEVICE_NAME "/dev/ttyUSB0"

#define GRAVITY 9.81
#define SIGN(x)	((x < 0) ? -1 : 1)

typedef enum
{
	OUTPUT_LV_ERROR = 0,
	OUTPUT_LV_WARNING = 2,
	OUTPUT_LV_QUIET = OUTPUT_LV_WARNING,
	OUTPUT_LV_PROCESS = 8,
	OUTPUT_LV_MODULE = 10,
	OUTPUT_LV_DEFAULT = OUTPUT_LV_MODULE,
	OUTPUT_LV_COMMAND = 12,
	OUTPUT_LV_PARAM = 13,
	OUTPUT_LV_CONTROL = 14,
	OUTPUT_LV_VERBOSE = OUTPUT_LV_CONTROL,
	OUTPUT_LV_DEBUG = 16,
} ParamOutputLv;

typedef struct _parameters *ParametersPtr;
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

int arg_analyze( int argc, char *argv[] );
void arg_help( int argc, char *argv[] );
void param_help( void );
void arg_longhelp( int argc, char *argv[] );
int set_param( char *filename, char *concrete_path );
int set_paramptr( FILE * paramfile );
void calc_param_inertia2ff( void );
void set_param_motor( void );
void set_param_velocity( void );
int parameter_set( char param, char id, long long int value64 );
int apply_robot_params( void );

int does_option_set( ParamOptions option );
int state( YPSpur_state id );
void enable_state( YPSpur_state id );
void disable_state( YPSpur_state id );
double p( YPSpur_param id, enum motor_id motor );
double isset_p( YPSpur_param id, enum motor_id motor );
double *pp( YPSpur_param id, enum motor_id motor );
ParametersPtr get_param_ptr(  );
int option( ParamOptions option );
ParamOutputLv output_lv( void );
void param_calc(  );

void param_update( void *filename );
void init_param_update_thread( pthread_t * thread, char *filename );
void param_update_loop_cleanup( void *data );

#endif
