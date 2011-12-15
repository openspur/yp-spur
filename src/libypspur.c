/* 
 * YP Spurとの通信ライブラリ Communication Library for SH Spur */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <ipcommunication.h>
#include <ypspur.h>

static int pid;
static int msq_id;
static int connection_error;

/* エラー確認 */
int YP_get_error_state(  )
{
	return connection_error;
}

/* coordinatorとのメッセージ通信を開始する */
int YPSpur_initex( int msq_key )
{

	/* メッセージ・キューのオープン */
	msq_id = msgget( msq_key, 0666 );

	if( msq_id == -1 ){
		connection_error = 1;
		return -1;
	}
	
	/* 内部データの初期化 */
	pid = 0x07fff & getpid(  );
	connection_error = 0;

	return 1;
}

/* coordinatorとのメッセージ通信を開始する */
int YPSpur_init( void )
{

	/* メッセージ・キューのオープン */
	msq_id = msgget( YPSPUR_MSQ_KEY, 0666 );
	
	if( msq_id == -1 ){
		connection_error = 1;
		return -1;
	}

	/* 内部データの初期化 */
	pid = 0x07fff & getpid(  );
	connection_error = 0;

	return 1;
}

/* 直線追従 */
int YPSpur_line( int cs, double x, double y, double theta )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_LINE;
	msg.data[0] = x;
	msg.data[1] = y;
	msg.data[2] = theta;
	msg.cs = cs;

	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}
	return 1;
}

/* 直線追従 */
int YPSpur_stop_line( int cs, double x, double y, double theta )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_STOP_LINE;
	msg.data[0] = x;
	msg.data[1] = y;
	msg.data[2] = theta;
	msg.cs = cs;

	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}
	return 1;
}

/* 円弧追従 */
int YPSpur_circle( int cs, double x, double y, double r )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_CIRCLE;
	msg.data[0] = x;
	msg.data[1] = y;
	msg.data[2] = r;
	msg.cs = cs;
	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	return 1;
}

/* 旋回 */
int YPSpur_spin( int cs, double theta )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_SPIN;
	msg.data[0] = theta;
	msg.cs = cs;
	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	return 1;
}

/* 方位 */
int YPSpur_orient( int cs, double theta )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_ORIENT;
	msg.data[0] = theta;
	msg.cs = cs;
	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	return 1;
}

/* 急ブレーキ */
int YPSpur_stop( void )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_STOP;

	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	return 1;
}

/* 緊急停止 */
int YPSpur_freeze( void )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_FREEZE;

	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	return 1;
}

/* 緊急停止解除 */
int YPSpur_unfreeze( void )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_UNFREEZE;

	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	return 1;
}

/* ソフトウェア補助フリーモード */
int YPSpur_free( void )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_FREE;

	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	return 1;
}

/* 位置指定 */
int YPSpur_set_pos( int cs, double x, double y, double theta )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_SET_POS;
	msg.data[0] = x;
	msg.data[1] = y;
	msg.data[2] = theta;
	msg.cs = cs;

	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}
	return 1;
}

/* 位置指定 */
int YPSpur_adjust_pos( int cs, double x, double y, double theta )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_ADJUST;
	msg.data[0] = x;
	msg.data[1] = y;
	msg.data[2] = theta;
	msg.cs = cs;

	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}
	return 1;
}

/* 速度指定 */
int YPSpur_set_vel( double v )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_SET_VEL;
	msg.data[0] = v;

	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	return 1;
}

/* 角速度指定 */
int YPSpur_set_angvel( double w )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_SET_ANGVEL;
	msg.data[0] = w;

	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	return 1;
}

/* 速度指定 */
int YPSpur_set_accel( double dv )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_SET_ACCEL;
	msg.data[0] = dv;

	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	return 1;
}

/* 角速度指定 */
int YPSpur_set_angaccel( double dw )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_SET_ANGACCEL;
	msg.data[0] = dw;

	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	return 1;
}

/* 位置取得 */
double YPSpur_get_pos( int cs, double *x, double *y, double *theta )
{
	YPSpur_msg msg;
	int len;
	double time;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_GET_POS;
	msg.cs = cs;
	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	/* 指定のコマンド受け取り */
	len = msgrcv( msq_id, &msg, YPSPUR_MSG_SIZE, pid, 0 );
	if( len < 0 )
	{
		/* receive error */
		connection_error = 1;
		return -1;
	}

	*x = msg.data[0];
	*y = msg.data[1];
	*theta = msg.data[2];
	time = msg.data[3];
	return time;
}

/* 速度取得 */
double YPSpur_get_vel( double *v, double *w )
{
	YPSpur_msg msg;
	int len;
	double time;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_GET_VEL;
	msg.cs = 0;
	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	/* 指定のコマンド受け取り */
	len = msgrcv( msq_id, &msg, YPSPUR_MSG_SIZE, pid, 0 );
	if( len < 0 )
	{
		/* receive error */
		connection_error = 1;
		return -1;
	}

	*v = msg.data[0];
	*w = msg.data[1];
	time = msg.data[2];
	return time;
}

/* 速度取得 */
double YP_get_wheel_vel( double *wr, double *wl )
{
	YPSpur_msg msg;
	int len;
	double time;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_GET_WHEEL_VEL;
	msg.cs = 0;
	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	/* 指定のコマンド受け取り */
	len = msgrcv( msq_id, &msg, YPSPUR_MSG_SIZE, pid, 0 );
	if( len < 0 )
	{
		/* receive error */
		connection_error = 1;
		return -1;
	}

	*wr = msg.data[0];
	*wl = msg.data[1];
	time = msg.data[2];
	return time;
}

/* 角度取得 */
double YP_get_wheel_ang( double *theta_r, double *theta_l )
{
	YPSpur_msg msg;
	int len;
	double time;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_GET_WHEEL_ANG;
	msg.cs = 0;
	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	/* 指定のコマンド受け取り */
	len = msgrcv( msq_id, &msg, YPSPUR_MSG_SIZE, pid, 0 );
	if( len < 0 )
	{
		/* receive error */
		connection_error = 1;
		return -1;
	}

	*theta_r = msg.data[0];
	*theta_l = msg.data[1];
	time = msg.data[2];
	return time;
}

/* トルク取得 */
double YP_get_wheel_torque( double *torque_r, double *torque_l )
{
	YPSpur_msg msg;
	int len;
	double time;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_GET_WHEEL_TORQUE;
	msg.cs = 0;
	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	/* 指定のコマンド受け取り */
	len = msgrcv( msq_id, &msg, YPSPUR_MSG_SIZE, pid, 0 );
	if( len < 0 )
	{
		/* receive error */
		connection_error = 1;
		return -1;
	}

	*torque_r = msg.data[0];
	*torque_l = msg.data[1];
	time = msg.data[2];
	return time;
}

/* 力取得 */
double YPSpur_get_force( double *trans, double *angular )
{
	YPSpur_msg msg;
	int len;
	double time;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_GET_FORCE;
	msg.cs = 0;
	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	/* 指定のコマンド受け取り */
	len = msgrcv( msq_id, &msg, YPSPUR_MSG_SIZE, pid, 0 );
	if( len < 0 )
	{
		/* receive error */
		connection_error = 1;
		return -1;
	}

	*trans = msg.data[0];
	*angular = msg.data[1];
	time = msg.data[2];
	return time;
}

/* 緊急停止状態取得 */
int YPSpur_isfreeze( void )
{
	YPSpur_msg msg;
	int len;
	int ret;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_ISFREEZE;
	msg.cs = 0;
	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	/* 指定のコマンド受け取り */
	len = msgrcv( msq_id, &msg, YPSPUR_MSG_SIZE, pid, 0 );
	if( len < 0 )
	{
		/* receive error */
		connection_error = 1;
		return -1;
	}

	ret = ( int )msg.data[0];
	return ret;
}

/* 直接速度入力 */
int YPSpur_vel( double v, double w )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_VEL;
	msg.data[0] = v;
	msg.data[1] = w;

	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	return 1;
}

/* 内部パラメータの変更 */
int YP_set_parameter( int param_id, double value )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_PARAM_SET;
	msg.cs = param_id;
	msg.data[0] = value;

	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	return 1;
}



/* 内部パラメータの取得 */
int YP_get_parameter( int param_id, double *value )
{
	YPSpur_msg msg;
	int len;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_PARAM_GET;
	msg.cs = param_id;
	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	/* 指定のコマンド受け取り */
	len = msgrcv( msq_id, &msg, YPSPUR_MSG_SIZE, pid, 0 );
	if( len < 0 )
	{
		/* receive error */
		connection_error = 1;
		return -1;
	}

	*value = msg.data[0];
	return msg.cs;
}

/* 内部状態の変更 */
int YP_set_control_state( int control_id, int state )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_PARAM_STATE;
	msg.cs = control_id;
	msg.data[0] = state;

	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	return 1;
}


/* 重力補償用地面の傾き指定 */
int YPSpur_tilt( int cs, double dir, double tilt )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_SET_TILT;
	msg.data[0] = dir;
	msg.data[1] = tilt;
	msg.cs = cs;

	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	return 1;
}

/* 位置判定 */
int YPSpur_near_pos( int cs, double x, double y, double r )
{
	YPSpur_msg msg;
	int len;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_NEAR_POS;
	msg.data[0] = x;
	msg.data[1] = y;
	msg.data[2] = r;
	msg.cs = cs;
	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	/* 指定のコマンド受け取り */
	len = msgrcv( msq_id, &msg, YPSPUR_MSG_SIZE, pid, 0 );
	if( len < 0 )
	{
		/* receive error */
		connection_error = 1;
		return -1;
	}

	return msg.cs;
}

/* 角度判定 */
int YPSpur_near_ang( int cs, double th, double d )
{
	YPSpur_msg msg;
	int len;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_NEAR_ANG;
	msg.data[0] = th;
	msg.data[1] = d;
	msg.cs = cs;
	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	/* 指定のコマンド受け取り */
	len = msgrcv( msq_id, &msg, YPSPUR_MSG_SIZE, pid, 0 );
	if( len < 0 )
	{
		/* receive error */
		connection_error = 1;
		return -1;
	}

	return msg.cs;
}

/* 領域判定 */
int YPSpur_over_line( int cs, double x, double y, double theta )
{
	YPSpur_msg msg;
	int len;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_OVER_LINE;
	msg.data[0] = x;
	msg.data[1] = y;
	msg.data[2] = theta;
	msg.cs = cs;
	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	/* 指定のコマンド受け取り */
	len = msgrcv( msq_id, &msg, YPSPUR_MSG_SIZE, pid, 0 );
	if( len < 0 )
	{
		/* receive error */
		connection_error = 1;
		return -1;
	}

	return msg.cs;
}

/* アナログ値取得 */
int YP_get_ad_value( int num )
{
	YPSpur_msg msg;
	int len;
	int ret;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_GETAD;
	msg.cs = 0;
	msg.data[0] = ( double )num;
	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	/* 指定のコマンド受け取り */
	len = msgrcv( msq_id, &msg, YPSPUR_MSG_SIZE, pid, 0 );
	if( len < 0 )
	{
		/* receive error */
		connection_error = 1;
		return -1;
	}

	ret = ( int )msg.data[0];
	return ret;
}

int YP_set_io_dir( unsigned char dir )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_SETIODIR;
	msg.data[0] = dir;

	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	return 1;
}

int YP_set_io_data( unsigned char data )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_SETIODATA;
	msg.data[0] = data;

	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	return 1;
}

/* 直接タイヤ回転速度入力 */
int YP_set_wheel_vel( double r, double l )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_WHEEL_VEL;
	msg.data[0] = r;
	msg.data[1] = l;

	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	return 1;
}

int YP_set_wheel_torque( double r, double l )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = pid;
	msg.type = YPSPUR_WHEEL_TORQUE;
	msg.data[0] = r;
	msg.data[1] = l;

	if( msgsnd( msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		connection_error = 1;
		return -1;
	}

	return 1;
}


