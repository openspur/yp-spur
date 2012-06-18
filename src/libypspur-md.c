/* 
 * YP-Spurとの通信ライブラリ複数デバイス対応版 Communication Library for YP-Spur */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define YP_SPUR_RUNTIME

#include <ipcommunication.h>
#include <ypspur-md.h>


/* エラー確認 */
int YP_md_get_error_state( YPSpur *dev )
{
	return dev->connection_error;
}

/* coordinatorとのメッセージ通信を開始する */
int YPSpur_md_initex( YPSpur *dev, int msq_key )
{

	/* メッセージ・キューのオープン */
	dev->msq_id = msgget( msq_key, 0666 );

	if( dev->msq_id == -1 ){
		dev->connection_error = 1;
		return -1;
	}
	
	/* 内部データの初期化 */
	dev->pid = 0x07fff & getpid(  );
	dev->connection_error = 0;

	return 1;
}

/* coordinatorとのメッセージ通信を開始する */
int YPSpur_md_init( YPSpur *dev )
{

	/* メッセージ・キューのオープン */
	dev->msq_id = msgget( YPSPUR_MSQ_KEY, 0666 );
	
	if( dev->msq_id == -1 ){
		dev->connection_error = 1;
		return -1;
	}

	/* 内部データの初期化 */
	dev->pid = 0x07fff & getpid(  );
	dev->connection_error = 0;

	return 1;
}

/* 直線追従 */
int YPSpur_md_line( YPSpur *dev, int cs, double x, double y, double theta )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_LINE;
	msg.data[0] = x;
	msg.data[1] = y;
	msg.data[2] = theta;
	msg.cs = cs;

	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}
	return 1;
}

/* 直線追従 */
int YPSpur_md_stop_line( YPSpur *dev, int cs, double x, double y, double theta )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_STOP_LINE;
	msg.data[0] = x;
	msg.data[1] = y;
	msg.data[2] = theta;
	msg.cs = cs;

	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}
	return 1;
}

/* 円弧追従 */
int YPSpur_md_circle( YPSpur *dev, int cs, double x, double y, double r )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_CIRCLE;
	msg.data[0] = x;
	msg.data[1] = y;
	msg.data[2] = r;
	msg.cs = cs;
	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	return 1;
}

/* 旋回 */
int YPSpur_md_spin( YPSpur *dev, int cs, double theta )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_SPIN;
	msg.data[0] = theta;
	msg.cs = cs;
	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	return 1;
}

/* 方位 */
int YPSpur_md_orient( YPSpur *dev, int cs, double theta )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_ORIENT;
	msg.data[0] = theta;
	msg.cs = cs;
	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	return 1;
}

/* 急ブレーキ */
int YPSpur_md_stop( YPSpur *dev )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_STOP;

	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	return 1;
}

/* 緊急停止 */
int YPSpur_md_freeze( YPSpur *dev )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_FREEZE;

	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	return 1;
}

/* 緊急停止解除 */
int YPSpur_md_unfreeze( YPSpur *dev )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_UNFREEZE;

	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	return 1;
}

/* ソフトウェア補助フリーモード */
int YPSpur_md_free( YPSpur *dev )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_FREE;

	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	return 1;
}

/* 位置指定 */
int YPSpur_md_set_pos( YPSpur *dev, int cs, double x, double y, double theta )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_SET_POS;
	msg.data[0] = x;
	msg.data[1] = y;
	msg.data[2] = theta;
	msg.cs = cs;

	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}
	return 1;
}

/* 位置指定 */
int YPSpur_md_adjust_pos( YPSpur *dev, int cs, double x, double y, double theta )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_ADJUST;
	msg.data[0] = x;
	msg.data[1] = y;
	msg.data[2] = theta;
	msg.cs = cs;

	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}
	return 1;
}

/* 速度指定 */
int YPSpur_md_set_vel( YPSpur *dev, double v )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_SET_VEL;
	msg.data[0] = v;

	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	return 1;
}

/* 角速度指定 */
int YPSpur_md_set_angvel( YPSpur *dev, double w )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_SET_ANGVEL;
	msg.data[0] = w;

	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	return 1;
}

/* 速度指定 */
int YPSpur_md_set_accel( YPSpur *dev, double dv )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_SET_ACCEL;
	msg.data[0] = dv;

	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	return 1;
}

/* 角速度指定 */
int YPSpur_md_set_angaccel( YPSpur *dev, double dw )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_SET_ANGACCEL;
	msg.data[0] = dw;

	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	return 1;
}

/* 位置取得 */
double YPSpur_md_get_pos( YPSpur *dev, int cs, double *x, double *y, double *theta )
{
	YPSpur_msg msg;
	int len;
	double time;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_GET_POS;
	msg.cs = cs;
	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	/* 指定のコマンド受け取り */
	len = msgrcv( dev->msq_id, &msg, YPSPUR_MSG_SIZE, dev->pid, 0 );
	if( len < 0 )
	{
		/* receive error */
		dev->connection_error = 1;
		return -1;
	}

	*x = msg.data[0];
	*y = msg.data[1];
	*theta = msg.data[2];
	time = msg.data[3];
	return time;
}

/* 速度取得 */
double YPSpur_md_get_vel( YPSpur *dev, double *v, double *w )
{
	YPSpur_msg msg;
	int len;
	double time;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_GET_VEL;
	msg.cs = 0;
	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	/* 指定のコマンド受け取り */
	len = msgrcv( dev->msq_id, &msg, YPSPUR_MSG_SIZE, dev->pid, 0 );
	if( len < 0 )
	{
		/* receive error */
		dev->connection_error = 1;
		return -1;
	}

	*v = msg.data[0];
	*w = msg.data[1];
	time = msg.data[2];
	return time;
}

/* 速度取得 */
double YP_md_get_wheel_vel( YPSpur *dev, double *wr, double *wl )
{
	YPSpur_msg msg;
	int len;
	double time;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_GET_WHEEL_VEL;
	msg.cs = 0;
	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	/* 指定のコマンド受け取り */
	len = msgrcv( dev->msq_id, &msg, YPSPUR_MSG_SIZE, dev->pid, 0 );
	if( len < 0 )
	{
		/* receive error */
		dev->connection_error = 1;
		return -1;
	}

	*wr = msg.data[0];
	*wl = msg.data[1];
	time = msg.data[2];
	return time;
}

/* 角度取得 */
double YP_md_get_wheel_ang( YPSpur *dev, double *theta_r, double *theta_l )
{
	YPSpur_msg msg;
	int len;
	double time;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_GET_WHEEL_ANG;
	msg.cs = 0;
	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	/* 指定のコマンド受け取り */
	len = msgrcv( dev->msq_id, &msg, YPSPUR_MSG_SIZE, dev->pid, 0 );
	if( len < 0 )
	{
		/* receive error */
		dev->connection_error = 1;
		return -1;
	}

	*theta_r = msg.data[0];
	*theta_l = msg.data[1];
	time = msg.data[2];
	return time;
}

/* トルク取得 */
double YP_md_get_wheel_torque( YPSpur *dev, double *torque_r, double *torque_l )
{
	YPSpur_msg msg;
	int len;
	double time;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_GET_WHEEL_TORQUE;
	msg.cs = 0;
	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	/* 指定のコマンド受け取り */
	len = msgrcv( dev->msq_id, &msg, YPSPUR_MSG_SIZE, dev->pid, 0 );
	if( len < 0 )
	{
		/* receive error */
		dev->connection_error = 1;
		return -1;
	}

	*torque_r = msg.data[0];
	*torque_l = msg.data[1];
	time = msg.data[2];
	return time;
}

/* 力取得 */
double YPSpur_md_get_force( YPSpur *dev, double *trans, double *angular )
{
	YPSpur_msg msg;
	int len;
	double time;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_GET_FORCE;
	msg.cs = 0;
	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	/* 指定のコマンド受け取り */
	len = msgrcv( dev->msq_id, &msg, YPSPUR_MSG_SIZE, dev->pid, 0 );
	if( len < 0 )
	{
		/* receive error */
		dev->connection_error = 1;
		return -1;
	}

	*trans = msg.data[0];
	*angular = msg.data[1];
	time = msg.data[2];
	return time;
}

/* 緊急停止状態取得 */
int YPSpur_md_isfreeze( YPSpur *dev )
{
	YPSpur_msg msg;
	int len;
	int ret;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_ISFREEZE;
	msg.cs = 0;
	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	/* 指定のコマンド受け取り */
	len = msgrcv( dev->msq_id, &msg, YPSPUR_MSG_SIZE, dev->pid, 0 );
	if( len < 0 )
	{
		/* receive error */
		dev->connection_error = 1;
		return -1;
	}

	ret = ( int )msg.data[0];
	return ret;
}

/* 直接速度入力 */
int YPSpur_md_vel( YPSpur *dev, double v, double w )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_VEL;
	msg.data[0] = v;
	msg.data[1] = w;

	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	return 1;
}

/* 内部パラメータの変更 */
int YP_md_set_parameter( YPSpur *dev, int param_id, double value )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_PARAM_SET;
	msg.cs = param_id;
	msg.data[0] = value;

	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	return 1;
}



/* 内部パラメータの取得 */
int YP_md_get_parameter( YPSpur *dev, int param_id, double *value )
{
	YPSpur_msg msg;
	int len;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_PARAM_GET;
	msg.cs = param_id;
	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	/* 指定のコマンド受け取り */
	len = msgrcv( dev->msq_id, &msg, YPSPUR_MSG_SIZE, dev->pid, 0 );
	if( len < 0 )
	{
		/* receive error */
		dev->connection_error = 1;
		return -1;
	}

	*value = msg.data[0];
	return msg.cs;
}

/* 内部状態の変更 */
int YP_md_set_control_state( YPSpur *dev, int control_id, int state )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_PARAM_STATE;
	msg.cs = control_id;
	msg.data[0] = state;

	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	return 1;
}


/* 重力補償用地面の傾き指定 */
int YPSpur_md_tilt( YPSpur *dev, int cs, double dir, double tilt )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_SET_TILT;
	msg.data[0] = dir;
	msg.data[1] = tilt;
	msg.cs = cs;

	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	return 1;
}

/* 位置判定 */
int YPSpur_md_near_pos( YPSpur *dev, int cs, double x, double y, double r )
{
	YPSpur_msg msg;
	int len;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_NEAR_POS;
	msg.data[0] = x;
	msg.data[1] = y;
	msg.data[2] = r;
	msg.cs = cs;
	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	/* 指定のコマンド受け取り */
	len = msgrcv( dev->msq_id, &msg, YPSPUR_MSG_SIZE, dev->pid, 0 );
	if( len < 0 )
	{
		/* receive error */
		dev->connection_error = 1;
		return -1;
	}

	return msg.cs;
}

/* 角度判定 */
int YPSpur_md_near_ang( YPSpur *dev, int cs, double th, double d )
{
	YPSpur_msg msg;
	int len;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_NEAR_ANG;
	msg.data[0] = th;
	msg.data[1] = d;
	msg.cs = cs;
	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	/* 指定のコマンド受け取り */
	len = msgrcv( dev->msq_id, &msg, YPSPUR_MSG_SIZE, dev->pid, 0 );
	if( len < 0 )
	{
		/* receive error */
		dev->connection_error = 1;
		return -1;
	}

	return msg.cs;
}

/* 領域判定 */
int YPSpur_md_over_line( YPSpur *dev, int cs, double x, double y, double theta )
{
	YPSpur_msg msg;
	int len;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_OVER_LINE;
	msg.data[0] = x;
	msg.data[1] = y;
	msg.data[2] = theta;
	msg.cs = cs;
	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	/* 指定のコマンド受け取り */
	len = msgrcv( dev->msq_id, &msg, YPSPUR_MSG_SIZE, dev->pid, 0 );
	if( len < 0 )
	{
		/* receive error */
		dev->connection_error = 1;
		return -1;
	}

	return msg.cs;
}

/* アナログ値取得 */
int YP_md_get_ad_value( YPSpur *dev, int num )
{
	YPSpur_msg msg;
	int len;
	int ret;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_GETAD;
	msg.cs = 0;
	msg.data[0] = ( double )num;
	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	/* 指定のコマンド受け取り */
	len = msgrcv( dev->msq_id, &msg, YPSPUR_MSG_SIZE, dev->pid, 0 );
	if( len < 0 )
	{
		/* receive error */
		dev->connection_error = 1;
		return -1;
	}

	ret = ( int )msg.data[0];
	return ret;
}

int YP_md_set_io_dir( YPSpur *dev, unsigned char dir )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_SETIODIR;
	msg.data[0] = dir;

	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	return 1;
}

int YP_md_set_io_data( YPSpur *dev, unsigned char data )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_SETIODATA;
	msg.data[0] = data;

	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	return 1;
}

/* 直接タイヤ回転速度入力 */
int YP_md_wheel_vel( YPSpur *dev, double r, double l )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_WHEEL_VEL;
	msg.data[0] = r;
	msg.data[1] = l;

	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	return 1;
}

int YP_md_wheel_torque( YPSpur *dev, double r, double l )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_WHEEL_TORQUE;
	msg.data[0] = r;
	msg.data[1] = l;

	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	return 1;
}

int YP_md_set_wheel_vel( YPSpur *dev, double r, double l )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_SET_WHEEL_VEL;
	msg.data[0] = r;
	msg.data[1] = l;

	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	return 1;
}

int YP_md_set_wheel_accel( YPSpur *dev, double r, double l )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_SET_WHEEL_ACCEL;
	msg.data[0] = r;
	msg.data[1] = l;

	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	return 1;
}

int YP_md_wheel_ang( YPSpur *dev, double r, double l )
{
	YPSpur_msg msg;

	msg.msg_type = YPSPUR_MSG_CMD;
	msg.pid = dev->pid;
	msg.type = YPSPUR_WHEEL_ANGLE;
	msg.data[0] = r;
	msg.data[1] = l;

	if( msgsnd( dev->msq_id, &msg, YPSPUR_MSG_SIZE, 0 ) < 0 )
	{
		/* error */
		dev->connection_error = 1;
		return -1;
	}

	return 1;
}


