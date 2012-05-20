/* high level I/O */
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <strings.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

/* low level I/O */
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* ボディパラメータ */
#include <shvel-param.h>

/* yp-spur用 */
#include <communication.h>
#include <param.h>
#include <control.h>
#include <command.h>
#include <utility.h>
#include <yprintf.h>
// #include <config.h>

/* ライブラリ用 */
#include <ypspur.h>

double g_P[YP_PARAM_NUM];
char g_state[YP_STATE_NUM];
Parameters g_param;

int state( YPSpur_state id )
{
	return ( g_state[id] == ENABLE );
}

void enable_state( YPSpur_state id )
{
	g_state[id] = ENABLE;
}

void disable_state( YPSpur_state id )
{
	g_state[id] = DISABLE;
}

double p( YPSpur_param id )
{
	return g_P[id];
}

double *pp( YPSpur_param id )
{
	return &g_P[id];
}

int option( ParamOptions option )
{
	if( g_param.option & option )
		return 1;
	return 0;
}

ParamOutputLv output_lv( void )
{
	return g_param.output_lv;
}

ParametersPtr get_param_ptr(  )
{
	return &g_param;
}

int is_character( int c );
int is_number( int c );

/* 引数の説明 */
void arg_help( int argc, char *argv[] )
{
	fprintf( stderr, "USAGE: %s [OPTION]...\n\n", argv[0] );
	fprintf( stderr, "  -v, --version            Display version info and exit.\n" );
	fprintf( stderr, "  -d, --device <device>    Specify a B-Loco device. e.g. /dev/ttyUSB0\n" );
	fprintf( stderr, "  -p, --param <tile>       Specify a robot parameter file.\n" );
	fprintf( stderr, "  --verbose                Display detail infomation.\n" );
	fprintf( stderr, "  --quiet                  Display nothing.\n" );
	fprintf( stderr, "  -h, --help               This help\n" );
	fprintf( stderr, "  --long-help              Long help\n" );
	fprintf( stderr, "  --param-help             Comments of parameters\n\n" );
}

/* 隠しコマンドの説明 */
void arg_longhelp( int argc, char *argv[] )
{
	arg_help( argc, argv );
	fprintf( stderr, "  -o, --show-odometry      Display estimated robot position.\n" );
	fprintf( stderr, "  -t, --show-timestamp     Display timestamp of serial communication \n"
	                 "                           with the B-Loco device.\n" );
	fprintf( stderr, "  --reconnect              Try reconnect device when device was closed.\n" );
	fprintf( stderr, "  --without-ssm            Run without ssm output.\n" );
	fprintf( stderr, "  -q, --msq-key <MSQKEY>   Run with message que key MSQKEY.\n" );
	fprintf( stderr, "  -s, --speed <BAUDRATE>   Set baudrate.\n" );
	fprintf( stderr, "  --admask <ADMASK>        Get AD data of ADMASK from B-Loco device.\n" );
	fprintf( stderr, "  --enable-get-digital-io  Enable digital IO port.\n" );
	fprintf( stderr, "  -c, --without-control    Run without control.\n" );
	fprintf( stderr, "  --without-device         Run without B-Loco device.\n" );
	fprintf( stderr, "  --no-yp-protocol         Run without checking plotocol of B-Loco device.\n" );
	fprintf( stderr, "  --passive                Passive run mode.\n" );

}


/* 引数の説明 */
void param_help( void )
{
	int i;
	char param_names[YP_PARAM_NUM][32] = YP_PARAM_NAME;
	char param_comments[YP_PARAM_NUM][128] = YP_PARAM_COMMENT;

	fprintf( stderr, "INFO: Comments of parameters (parameter version %.1f)\n\n", YP_PARAM_REQUIRED_VERSION );
	for( i = 0; i < YP_PARAM_NUM; i ++ )
	{
		if( param_names[i][0] == '_' ) continue;
		fprintf( stderr, "  %20s: %s\n", param_names[i], param_comments[i] );
	}
}

/* 引数の解析 */
int arg_analyze( int argc, char *argv[] )
{
	int i;

	g_param.option = 0;
	g_param.msq_key = YPSPUR_MSQ_KEY;
	g_param.output_lv = OUTPUT_LV_DEFAULT;
	g_param.speed = 0;

	strcpy( g_param.parameter_filename, DEFAULT_PARAMETER_FILE );
	strcpy( g_param.device_name, DEFAULT_DEVICE_NAME );

	for ( i = 1; i < argc; i++ )
	{
		if( !strcmp( argv[i], "--help" ) || !strcmp( argv[i], "-h" ) )
		{
			g_param.option |= OPTION_SHOW_HELP;
		}
		else if( !strcmp( argv[i], "--long-help" ) )
		{
			g_param.option |= OPTION_SHOW_LONGHELP;
		}
		else if( !strcmp( argv[i], "--param-help" ) )
		{
			g_param.option |= OPTION_SHOW_PARAMHELP;
		}
		else if( !strcmp( argv[i], "--show-odometry" ) || !strcmp( argv[i], "-o" ) )
		{
			g_param.option |= OPTION_SHOW_ODOMETRY;
		}
		else if( !strcmp( argv[i], "--show-timestamp" ) || !strcmp( argv[i], "-t" ) )
		{
			g_param.option |= OPTION_SHOW_TIMESTAMP;
		}
		else if( !strcmp( argv[i], "--param" ) || !strcmp( argv[i], "-p" ) )
		{
			if( i + 1 < argc )
			{
				i++;
				strcpy( g_param.parameter_filename, argv[i] );
			}
			else
				break;
		}
		else if( !strcmp( argv[i], "--admask" ) )
		{
			if( i + 1 < argc )
			{
				char *pos;

				i++;
				g_param.admask = 0;
				for ( pos = argv[i]; *pos != 0; pos++ )
				{
					g_param.admask = g_param.admask << 1;
					if( *pos == '1' )
						g_param.admask |= 1;
				}
			}
			else
				break;
		}
		else if( !strcmp( argv[i], "--device" ) || !strcmp( argv[i], "-d" ) )
		{
			if( i + 1 < argc )
			{
				i++;
				strcpy( g_param.device_name, argv[i] );
			}
			else
				break;
		}
		else if( !strcmp( argv[i], "--without-control" ) || !strcmp( argv[i], "-c" ) )
		{
			g_param.option |= OPTION_PARAM_CONTROL;
			disable_state( YP_STATE_MOTOR );
			disable_state( YP_STATE_VELOCITY );
			disable_state( YP_STATE_BODY );
			disable_state( YP_STATE_TRACKING );
		}
		else if( !strcmp( argv[i], "--without-device" ) )
		{
			g_param.option |= OPTION_WITHOUT_DEVICE;
			g_param.option |= OPTION_DO_NOT_USE_YP;
			g_param.option |= OPTION_PARAM_CONTROL;
			disable_state( YP_STATE_MOTOR );
			disable_state( YP_STATE_VELOCITY );
			disable_state( YP_STATE_BODY );
			disable_state( YP_STATE_TRACKING );
		}
		else if( !strcmp( argv[i], "--version" ) || !strcmp( argv[i], "-v" ) )
		{
			g_param.option |= OPTION_VERSION;
		}
		else if( !strcmp( argv[i], "--verbose" ) )
		{
			g_param.output_lv = OUTPUT_LV_VERBOSE;
		}
		else if( !strcmp( argv[i], "--quiet" ) )
		{
			g_param.output_lv = OUTPUT_LV_QUIET;
		}
		else if( !strcmp( argv[i], "--reconnect" ) )
		{
			g_param.option |= OPTION_RECONNECT;
		}
		else if( !strcmp( argv[i], "--enable-set-bs" ) )
		{
			g_param.option |= OPTION_ENABLE_SET_BS;
		}
		else if( !strcmp( argv[i], "--enable-get-digital-io" ) )
		{
			g_param.option |= OPTION_ENABLE_GET_DIGITAL_IO;
		}
		else if( !strcmp( argv[i], "--no-yp-protocol" ) )
		{
			g_param.option |= OPTION_DO_NOT_USE_YP;
		}
		else if( !strcmp( argv[i], "--without-ssm" ) )
		{
			g_param.option |= OPTION_WITHOUT_SSM;
		}
		else if( !strcmp( argv[i], "--msq-key" ) || !strcmp( argv[i], "-q" ) )
		{
			if( i + 1 < argc )
			{
				i++;
				g_param.msq_key = atoi( argv[i] );
			}
			else
				break;
		}
		else if( !strcmp( argv[i], "--speed" ) || !strcmp( argv[i], "-s" ) )
		{
			if( i + 1 < argc )
			{
				i++;
				g_param.speed = atoi( argv[i] );
			}
			else
				break;
		}
		else if( !strcmp( argv[i], "--passive" ) )
		{
			g_param.option |= OPTION_PASSIVE;
		}
		else
		{
			yprintf( OUTPUT_LV_ERROR, "ERROR : invalid option -- '%s'.\n", argv[i] );
			return 0;
		}
	}

	if( i < argc )
	{
		yprintf( OUTPUT_LV_ERROR, "ERROR :  option requires an argument -- '%s'.\n", argv[i] );
		return 0;
	}

	return 1;
}

/* parameter set command */
int parameter_set( char param, char id, int value )
{
	char buf[7];
	buf[0] = param;
	buf[1] = id;
	buf[2] = ( ( Int_4Char ) value ).byte[3];
	buf[3] = ( ( Int_4Char ) value ).byte[2];
	buf[4] = ( ( Int_4Char ) value ).byte[1];
	buf[5] = ( ( Int_4Char ) value ).byte[0];
	encode_write( buf, 6 );

	return ( 0 );
}

int is_character( int c )
{
	if( c >= 'A' && c <= 'Z' )
		return 1;
	if( c >= 'a' && c <= 'z' )
		return 1;
	if( c == '_' )
		return 1;
	return 0;
}

int is_number( int c )
{
	if( c >= '0' && c <= '9' )
		return 1;
	if( c == '-' )
		return 1;
	if( c == '.' )
		return 1;
	return 0;
}

/* パラメータファイルからの読み込み */
int set_param( char *filename )
{
	FILE *paramfile;
	char param_names[YP_PARAM_NUM][20] = YP_PARAM_NAME;
	char name[20], value_str[100];
	int i, c;
	// double value;
	int str_wp;
	int read_state;

	/* - */
	paramfile = fopen( filename, "r" );

	if( !paramfile )
	{
		char dir_name[256];
		char file_name[256];
		char *pret;
		FILE *fd;

		yprintf( OUTPUT_LV_PARAM, "Warn: File [%s] is not exist.\n", filename );

		if( !strchr( filename, '/' ) )
		{
			/* ファイルが見つからないとき、かつパス指定でないときshareディレクトリを見に行く 
			 */
			fd = popen( "pkg-config --variable=YP_PARAMS_DIR yp-robot-params", "r" );
			if( ( fd == NULL ) )
			{
				yprintf( OUTPUT_LV_ERROR,
							 "Error: Cannot open pipe 'pkg-config --variable=YP_PARAMS_DIR yp-robot-params'.\n" );
				return 0;
			}
			pret = fgets( dir_name, sizeof ( dir_name ), fd );

			if( ( pclose( fd ) == 0 ) && ( pret != 0 ) )
			{
				dir_name[strlen( dir_name ) - 1] = '\0';
				// printf( "dir = '%s'\n", dir_name );
				sprintf( file_name, "%s/%s", dir_name, filename );

				paramfile = fopen( file_name, "r" );

				if( !paramfile )
				{
					yprintf( OUTPUT_LV_WARNING, "Warn: File [%s] is not exist.\n", file_name );
					return 0;
				}
			}
			else
			{
				yprintf( OUTPUT_LV_ERROR,
							 "Error: Cannot read pipe 'pkg-config --variable=YP_PARAMS_DIR yp-robot-params'.\n" );
				return 0;
			}
			// fprintf( stdout, "open %s\n", file_name );
			strcpy( file_name, filename );
		}
		else
		{
			return 0;
		}
	}

	// パラメータの初期化
	for ( i = 0; i < YP_PARAM_NUM; i++ )
	{
		g_P[i] = 0;
	}

	// パラメータファイルの読み込み
	read_state = 0;
	str_wp = 0;
	while( ( c = getc( paramfile ) ) != EOF )
	{
		switch ( read_state )
		{
		case 0:
			/* - */
			if( c == '#' )
			{
				read_state = 1;
			}
			if( is_character( c ) )
			{
				name[0] = c;
				read_state = 2;
				str_wp = 1;
			}
			break;
		case 1:								/* comment */
			if( c == '\n' )
			{
				read_state = 0;
			}
			break;
		case 2:								/* name */
			name[str_wp] = c;
			if( !( is_character( c ) || is_number( c ) ) )
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
		case 3:								/* value */
			if( is_number( c ) )
			{
				str_wp = 0;
				value_str[str_wp] = c;
				str_wp++;
				read_state = 4;
			}
			break;
		case 4:								/* value */
			value_str[str_wp] = c;
			if( !is_number( c ) )
			{
				value_str[str_wp] = 0;
				read_state = 0;
				// printf("%s\n",name);
				for ( i = 0; i < YP_PARAM_NUM; i++ )
				{
					if( !strcmp( name, param_names[i] ) )
					{
						g_P[i] = strtod( value_str, 0 );
						yprintf( OUTPUT_LV_PARAM, "%d %s %f\n", i,name, g_P[i] );
						break;
					}
				}
				if( i == YP_PARAM_NUM )
					yprintf( OUTPUT_LV_WARNING, "Warn: invalid parameter -- '%s'.\n", name );
			}
			else
			{
				str_wp++;
			}
		}
	}
	if( g_P[YP_PARAM_VERSION] < YP_PARAM_REQUIRED_VERSION )
	{
		yprintf( OUTPUT_LV_ERROR, "Error: Your parameter file format is too old!\n" );
		yprintf( OUTPUT_LV_ERROR, "Error: This program require v%3.1f.\n", YP_PARAM_REQUIRED_VERSION );
		return 0;
	}
	if( g_P[YP_PARAM_VERSION] > YP_PARAM_REQUIRED_VERSION )
	{
		yprintf( OUTPUT_LV_ERROR, "Error: Your parameter file format is unsupported!\n" );
		yprintf( OUTPUT_LV_ERROR, "Error: Please install newer version of YP-Spur.\n" );
		yprintf( OUTPUT_LV_ERROR, "Error: This program require v%3.1f.\n", YP_PARAM_REQUIRED_VERSION );
		return 0;
	}


	// パラメータの指定によって自動的に求まるパラメータの計算
	calc_param_inertia2ff();
	
	/* パラメータの入力 */
	// parameter_set(PARAM_free,0,0); 

	// robot_speed( 0, 0 );

	// apply_robot_params( );

	/* パラメータを有効にする */
	enable_state( YP_STATE_MOTOR );
	enable_state( YP_STATE_VELOCITY );
	enable_state( YP_STATE_BODY );
	enable_state( YP_STATE_TRACKING );

	// printf( " mass %f\n", g_P[YP_PARAM_MASS] );
	return 1;
}

/* パラメータ適用 */
int apply_robot_params(  )
{
	robot_speed( 0, 0 );

	/* モータのパラメータ */
	set_param_motor(  );
	yp_usleep( 100000 );

	/* 速度制御パラメータ */
	set_param_velocity(  );
	yp_usleep( 100000 );

	return 1;
}

// FF制御パラメータの計算
// 坪内　孝司　車輪移動体の制御　を参照
void calc_param_inertia2ff( void )
{
	int i;
	double A, B, C, D, E, F;					// 制御パラメータ
	double M, J;								// ロボットの質量、慣性モーメント
	double Gr, Gl;								// ギア比
	double Jmr, Jml;							// モータの慣性モーメント
	double Jtr, Jtl;							// タイヤの慣性モーメント
	double Rr, Rl;								// タイヤ半径
	double T;									// トレッド
	double fr, fl;								// 動摩擦係数
	
	// パラメータの代入
	M = g_P[YP_PARAM_MASS];
	J = g_P[YP_PARAM_MOMENT_INERTIA];
	Gr = Gl = g_P[YP_PARAM_GEAR];
	Jmr = Jml = g_P[YP_PARAM_MOTOR_M_INERTIA];
	Jtr = Jtl = g_P[YP_PARAM_TIRE_M_INERTIA];
	Rr = g_P[YP_PARAM_RADIUS_R];
	Rl = g_P[YP_PARAM_RADIUS_L];
	T = g_P[YP_PARAM_TREAD];
	fl = fr = 0.0;	// モータ制御器の中に摩擦補償が入っているのでいらない？
	
//	printf("M = %lf\n",M);
//	printf("J = %lf\n",J);
	// パラメータの計算
	A = ( Gr*Gr*Jmr + Jtr + Rr*Rr/2.0*( M/2.0 + J/( T*T ) ) ) / Gr;
	B = ( Gl*Gl*Jml + Jtl + Rl*Rl/2.0*( M/2.0 + J/( T*T ) ) ) / Gl;
	C = ( Rr*Rl/2.0 * ( M/2.0 - J/( T*T ) ) ) / Gr;
	D = ( Rr*Rl/2.0 * ( M/2.0 - J/( T*T ) ) ) / Gl;
	E = fr / Gr;
	F = fl / Gl;
	
	// パラメータの設定
	g_P[YP_PARAM_GAIN_A] = A;
	g_P[YP_PARAM_GAIN_B] = B;
	g_P[YP_PARAM_GAIN_C] = C;
	g_P[YP_PARAM_GAIN_D] = D;
	g_P[YP_PARAM_GAIN_E] = E;
	g_P[YP_PARAM_GAIN_F] = F;
	
	// 出力（デバッグ）
	for(i = 0; i < 6; i++ )
	{
		yprintf( OUTPUT_LV_PARAM, " %c %f\n", 'A' + i, g_P[YP_PARAM_GAIN_A + i] );
	}
}

// モータパラメータの送信
void set_param_motor( void )
{
	double tvc; /* 変換用定数 */
	/* モータのパラメータ */
	parameter_set( PARAM_p_ki, 0,
				   ( double )( 65536.0 * g_P[YP_PARAM_PWM_MAX] * g_P[YP_PARAM_MOTOR_R] /
							   ( g_P[YP_PARAM_TORQUE_UNIT] * g_P[YP_PARAM_MOTOR_TC] * g_P[YP_PARAM_VOLT] ) ) );
	parameter_set( PARAM_p_ki, 1,
				   ( double )( 65536.0 * g_P[YP_PARAM_PWM_MAX] * g_P[YP_PARAM_MOTOR_R] /
							   ( g_P[YP_PARAM_TORQUE_UNIT] * g_P[YP_PARAM_MOTOR_TC] * g_P[YP_PARAM_VOLT] ) ) );
	parameter_set( PARAM_p_kv, 0,
				   ( double )( 65536.0 * g_P[YP_PARAM_PWM_MAX] * 60.0 /
							   ( g_P[YP_PARAM_MOTOR_VC] * g_P[YP_PARAM_VOLT] * g_P[YP_PARAM_COUNT_REV] *
								 g_P[YP_PARAM_CYCLE] ) ) );
	parameter_set( PARAM_p_kv, 1,
				   ( double )( 65536.0 * g_P[YP_PARAM_PWM_MAX] * 60.0 /
							   ( g_P[YP_PARAM_MOTOR_VC] * g_P[YP_PARAM_VOLT] * g_P[YP_PARAM_COUNT_REV] *
								 g_P[YP_PARAM_CYCLE] ) ) );
	/* 摩擦補償 */
	parameter_set( PARAM_p_fr_plus, 0, g_P[YP_PARAM_TORQUE_NEWTON] * g_P[YP_PARAM_TORQUE_UNIT] );
	parameter_set( PARAM_p_fr_plus, 1, g_P[YP_PARAM_TORQUE_NEWTON] * g_P[YP_PARAM_TORQUE_UNIT] );
	parameter_set( PARAM_p_fr_minus, 0, g_P[YP_PARAM_TORQUE_NEWTON] * g_P[YP_PARAM_TORQUE_UNIT] );
	parameter_set( PARAM_p_fr_minus, 1, g_P[YP_PARAM_TORQUE_NEWTON] * g_P[YP_PARAM_TORQUE_UNIT] );
	// 1/(rad/s) -> 1/(count/1ms)
	tvc = ( g_P[YP_PARAM_COUNT_REV] / 2.0 * M_PI ) * g_P[YP_PARAM_CYCLE];
	parameter_set( PARAM_p_fr_wplus, 0, g_P[YP_PARAM_TORQUE_VISCOS] * g_P[YP_PARAM_TORQUE_UNIT] * tvc );
	parameter_set( PARAM_p_fr_wplus, 1, g_P[YP_PARAM_TORQUE_VISCOS] * g_P[YP_PARAM_TORQUE_UNIT] * tvc );
	parameter_set( PARAM_p_fr_wminus, 0, g_P[YP_PARAM_TORQUE_VISCOS] * g_P[YP_PARAM_TORQUE_UNIT] * tvc );
	parameter_set( PARAM_p_fr_wminus, 1, g_P[YP_PARAM_TORQUE_VISCOS] * g_P[YP_PARAM_TORQUE_UNIT] * tvc );

	parameter_set( PARAM_p_toq_offset, 0, g_P[YP_PARAM_TORQUE_OFFSET] * g_P[YP_PARAM_TORQUE_UNIT] );
	parameter_set( PARAM_p_toq_offset, 1, g_P[YP_PARAM_TORQUE_OFFSET] * g_P[YP_PARAM_TORQUE_UNIT] );

	parameter_set( PARAM_toq_max, 0, g_P[YP_PARAM_TORQUE_MAX] * g_P[YP_PARAM_TORQUE_UNIT] );
	parameter_set( PARAM_toq_max, 1, g_P[YP_PARAM_TORQUE_MAX] * g_P[YP_PARAM_TORQUE_UNIT] );
	parameter_set( PARAM_toq_min, 0, -g_P[YP_PARAM_TORQUE_MAX] * g_P[YP_PARAM_TORQUE_UNIT] );
	parameter_set( PARAM_toq_min, 1, -g_P[YP_PARAM_TORQUE_MAX] * g_P[YP_PARAM_TORQUE_UNIT] );

	parameter_set( PARAM_pwm_max, 0, g_P[YP_PARAM_PWM_MAX] );
	parameter_set( PARAM_pwm_max, 1, g_P[YP_PARAM_PWM_MAX] );
	parameter_set( PARAM_pwm_min, 0, -g_P[YP_PARAM_PWM_MAX] );
	parameter_set( PARAM_pwm_min, 1, -g_P[YP_PARAM_PWM_MAX] );

}

void set_param_velocity( void )
{
	double ff;
	/* ウォッチドックタイマの設定 */
	parameter_set( PARAM_watch_dog_limit, 0, 300 );

	/* FF制御パラメータ */
	/* カウンタ値から角速度に直すための固定値(8bit固定小数点的な計算) & トルク計算のためのオフセット */
	ff = 2.0 * M_PI / g_P[YP_PARAM_COUNT_REV] * 256.0 * g_P[YP_PARAM_TORQUE_UNIT];
	parameter_set( PARAM_p_A, 0, g_P[YP_PARAM_GAIN_A] * ff );
	parameter_set( PARAM_p_B, 0, g_P[YP_PARAM_GAIN_B] * ff );
	parameter_set( PARAM_p_C, 0, g_P[YP_PARAM_GAIN_C] * ff );
	parameter_set( PARAM_p_D, 0, g_P[YP_PARAM_GAIN_D] * ff );
	parameter_set( PARAM_p_E, 0, g_P[YP_PARAM_GAIN_E] * ff );
	parameter_set( PARAM_p_F, 0, g_P[YP_PARAM_GAIN_F] * ff );
	/* PI制御のパラメータ */
	/* [1/s] -> [1/ms] & トルク計算のためのオフセット */
	parameter_set( PARAM_p_pi_kp, 0, g_P[YP_PARAM_GAIN_KP] * g_P[YP_PARAM_TORQUE_UNIT] * g_P[YP_PARAM_CYCLE] );
	parameter_set( PARAM_p_pi_kp, 1, g_P[YP_PARAM_GAIN_KP] * g_P[YP_PARAM_TORQUE_UNIT] * g_P[YP_PARAM_CYCLE] );
	/* [1/s^2] -> [1/ms^2] & トルク計算のためのオフセット */
	parameter_set( PARAM_p_pi_ki, 0, g_P[YP_PARAM_GAIN_KI] * g_P[YP_PARAM_TORQUE_UNIT] * g_P[YP_PARAM_CYCLE]  * g_P[YP_PARAM_CYCLE] );
	parameter_set( PARAM_p_pi_ki, 1, g_P[YP_PARAM_GAIN_KI] * g_P[YP_PARAM_TORQUE_UNIT] * g_P[YP_PARAM_CYCLE]  * g_P[YP_PARAM_CYCLE] );
	/* 各種制限 */
	/* rev -> cnt */
	parameter_set( PARAM_int_max, 0, g_P[YP_PARAM_INTEGRAL_MAX] * g_P[YP_PARAM_COUNT_REV] * g_P[YP_PARAM_GEAR] );
	parameter_set( PARAM_int_max, 1, g_P[YP_PARAM_INTEGRAL_MAX] * g_P[YP_PARAM_COUNT_REV] * g_P[YP_PARAM_GEAR] );
	parameter_set( PARAM_int_min, 0, -g_P[YP_PARAM_INTEGRAL_MAX] * g_P[YP_PARAM_COUNT_REV] * g_P[YP_PARAM_GEAR] );
	parameter_set( PARAM_int_min, 1, -g_P[YP_PARAM_INTEGRAL_MAX] * g_P[YP_PARAM_COUNT_REV] * g_P[YP_PARAM_GEAR] );

	parameter_set( PARAM_w_ref, 0, 0 );
	parameter_set( PARAM_w_ref, 1, 0 );

	if( option( OPTION_PASSIVE ) )
	{
		parameter_set( PARAM_servo, 0, SERVO_LEVEL_TORQUE );
	}
	else
	{
		parameter_set( PARAM_servo, 0, SERVO_LEVEL_VELOCITY );
	}
}

void motor_stop( void )
{
	parameter_set( PARAM_servo, 0, SERVO_LEVEL_COUNTER );
}

void motor_free( void )
{
	parameter_set( PARAM_servo, 0, SERVO_LEVEL_TORQUE );
}

void motor_servo( void )
{
	if( option( OPTION_PASSIVE ) )
	{
		parameter_set( PARAM_servo, 0, SERVO_LEVEL_TORQUE );
	}
	else
	{
		parameter_set( PARAM_servo, 0, SERVO_LEVEL_VELOCITY );
	}
}

