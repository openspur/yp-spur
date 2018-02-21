/* high level I/O */
#include <stdio.h>
#include <math.h>
#include <unistd.h>
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
#include <param.h>
#include <control.h>
#include <adinput.h>
#include <utility.h>
#include <yprintf.h>
#include <communication.h>
#include <serial.h>
#include <ssm_spur_handler.h>

/* ライブラリ用 */
#include <ypspur.h>
#include <cartesian2d.h>


double g_interval;
double g_offset;
int g_offset_point;
double g_estimated_delay = 0;

CSptr g_GL;
CSptr g_SP;
CSptr g_LC;
CSptr g_BS;
CSptr g_FS;
CSptr g_BL;

Odometry g_odometry;

/* CS の初期化 */
void init_coordinate_systems( void )
{
	/* */
	g_BS = CS_add( 0, 0, 0, 0 );				/* オドメトリ座標系 */
	g_GL = CS_add( g_BS, 0, 0, 0 );				/* グローバル座標系 */
	g_LC = CS_add( g_GL, 0, 0, 0 );				/* ローカル座標系 */
	g_FS = CS_add( g_BS, 0, 0, 0 );				/* 自己位置 */
	g_BL = CS_add( g_BS, 0, 0, 0 );				/* オドメトリローカル座標系 */
	g_SP = CS_add( g_BS, 0, 0, 0 );				/* Spur座標系（走行制御用） */
}

void init_odometry( void )
{
	int i;
	g_odometry.x = 0;
	g_odometry.y = 0;
	g_odometry.theta = 0;
	for(i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i ++)
	{
		g_odometry.wang[0] = 0;
		g_odometry.wtorque[0] = 0;
		g_odometry.wvel[0] = 0;
	}
	g_odometry.v = 0;
	g_odometry.w = 0;
	g_odometry.time = 0;
	g_offset_point = 0;
}

CSptr get_cs_pointer( YPSpur_cs cs )
{
	switch ( cs )
	{
	case CS_FS:
		return g_FS;
		break;
	case CS_LC:
		return g_LC;
		break;
	case CS_GL:
		return g_GL;
		break;
	case CS_SP:
		return g_SP;
		break;
	case CS_BS:
		return g_BS;
		break;
	case CS_BL:
		return g_BL;
		break;
	default:
		return NULL;
		break;
	}
	return NULL;
}

void cstrans_xy( YPSpur_cs src, YPSpur_cs dest, double *x, double *y, double *theta )
{
	if( src == dest )
		return;
	CS_recursive_trans( get_cs_pointer( dest ), get_cs_pointer( src ), x, y, theta );
}

void set_cs( YPSpur_cs cs, double x, double y, double theta )
{
	CS_set( get_cs_pointer( cs ), x, y, theta );
}

/* オドメトリ計算 */
void odometry( OdometryPtr xp, short *cnt, short *pwm, double dt, double time )
{
	double v, w;
	double wvel[YP_PARAM_MAX_MOTOR_NUM], mvel[YP_PARAM_MAX_MOTOR_NUM];
	double mtorque[YP_PARAM_MAX_MOTOR_NUM], wtorque[YP_PARAM_MAX_MOTOR_NUM];
	double volt[YP_PARAM_MAX_MOTOR_NUM], vc[YP_PARAM_MAX_MOTOR_NUM];
	double torque_trans, torque_angular;
	Parameters *param;
	param = get_param_ptr();

	int i;
	for(i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i ++)
	{
		if(!param->motor_enable[i]) continue;

		short cnt_diff;
		if(param->device_version > 8)
		{
			cnt_diff = (short)cnt[i] - (short)xp->enc[i];
			xp->enc[i] = cnt[i];
		}
		else
		{
			cnt_diff = cnt[i];
			xp->enc[i] += cnt_diff;
		}

		/* 角速度計算 */
		mvel[i] = 2.0 * M_PI * ( ( double )cnt_diff ) * pow(2, p( YP_PARAM_ENCODER_DIV, i ))
			/ ( p( YP_PARAM_COUNT_REV, i ) * dt );
		wvel[i] = mvel[i] / p( YP_PARAM_GEAR, i );

		/* トルク推定 */
		volt[i] = ( double )pwm[i] * p( YP_PARAM_VOLT, i )
			 / ( p( YP_PARAM_PWM_MAX, i ) * ( dt / p( YP_PARAM_CYCLE, i ) ) );
		vc[i] = ( p( YP_PARAM_MOTOR_VC, i ) / 60 ) * 2 * M_PI; // [rpm/V] => [(rad/s)/V]
		// TC [Nm/A]
		mtorque[i] = p( YP_PARAM_MOTOR_TC, i ) 
			* ( ( volt[i] - mvel[i] / vc[i] ) / p( YP_PARAM_MOTOR_R, i ) );
		/* 摩擦補償の補償 */
		if( wvel[i] > 0 )
		{
			mtorque[i] -= p( YP_PARAM_TORQUE_NEWTON, i )
					   + p( YP_PARAM_TORQUE_VISCOS, i ) * fabs( mvel[i] );
		}
		else if( wvel[i] < 0 )
		{
			mtorque[i] += p( YP_PARAM_TORQUE_NEWTON_NEG, i )
					   + p( YP_PARAM_TORQUE_VISCOS_NEG, i ) * fabs( mvel[i] );
		}
		wtorque[i] = mtorque[i] * p( YP_PARAM_GEAR, i );
	}

	/* キネマティクス計算 */
	v = p( YP_PARAM_RADIUS, MOTOR_RIGHT ) * wvel[MOTOR_RIGHT] / 2.0
	   	+ p( YP_PARAM_RADIUS, MOTOR_LEFT ) * wvel[MOTOR_LEFT] / 2.0;
	w = +p( YP_PARAM_RADIUS, MOTOR_RIGHT ) * wvel[MOTOR_RIGHT] / p( YP_PARAM_TREAD, 0 )
	    -p( YP_PARAM_RADIUS, MOTOR_LEFT ) * wvel[MOTOR_LEFT] / p( YP_PARAM_TREAD, 0 );

	torque_trans = wtorque[MOTOR_RIGHT] / p( YP_PARAM_RADIUS, MOTOR_RIGHT )
	               + wtorque[MOTOR_LEFT] / p( YP_PARAM_RADIUS, MOTOR_LEFT );
	torque_angular = ( + wtorque[MOTOR_RIGHT] / p( YP_PARAM_RADIUS, MOTOR_RIGHT )
					   - wtorque[MOTOR_LEFT] / p( YP_PARAM_RADIUS, MOTOR_LEFT ) )
	                   * p( YP_PARAM_TREAD, 0 ) / 2;

	/* オドメトリ計算 */
	xp->x = xp->x + v * cos( xp->theta ) * dt;
	xp->y = xp->y + v * sin( xp->theta ) * dt;
	xp->theta = xp->theta + w * dt;
	xp->time = time;
	if( option( OPTION_SHOW_TIMESTAMP ) ) {
		static int cnt = 0;
		if(++cnt % 100 == 0)
			printf("now - stamp: %0.3f ms\n", (get_time() - time) * 1000.0);
	}
	xp->v = v;
	xp->w = w;
	for(i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i ++)
	{
		if(!param->motor_enable[i]) continue;
		xp->wvel[i] = wvel[i];
		xp->wang[i] = xp->wang[i] + xp->wvel[i] * dt;
		xp->wtorque[i] = wtorque[i];
	}
	xp->torque_trans = torque_trans;
	xp->torque_angular = torque_angular;


  /*-PI< <PIに調整*/
	// if(xp->theta <-M_PI)xp->theta += 2*M_PI;
	// if(xp->theta > M_PI)xp->theta -= 2*M_PI;

	/* FS座標系セット */
	CS_set( g_FS, xp->x, xp->y, xp->theta );
	
	// 数式指定のパラメータを評価
	param_calc( );
}

/* 割り込み型データの処理 */
void process_int( OdometryPtr xp, int param_id, int id, int value )
{
	Parameters *param;
	param = get_param_ptr();

	if(!param->motor_enable[id]) return;

	switch( param_id )
	{
	case INT_enc_index_rise:
	case INT_enc_index_fall:
		{
			// enc == value のときに INDEX_RISE/FALL_ANGLE [rad] だった
			const unsigned short enc_div = 
				((unsigned int)xp->enc[id] << ((int)p( YP_PARAM_ENCODER_DIV, id ))) & 0xFFFF;
			const short enc_diff = (short)enc_div - (short)value;
			const double ang_diff = enc_diff * 2.0 * M_PI / 
				( p( YP_PARAM_COUNT_REV, id ) * p( YP_PARAM_GEAR, id ) );

			const double index_ratio = p( YP_PARAM_INDEX_GEAR, id ) / p( YP_PARAM_GEAR, id );
			double ref_ang;
			if( param_id == INT_enc_index_rise )
			{
				if( !isset_p( YP_PARAM_INDEX_RISE_ANGLE, id ) ) break;
				ref_ang = p( YP_PARAM_INDEX_RISE_ANGLE, id );
			}
			else
			{
				if( !isset_p( YP_PARAM_INDEX_FALL_ANGLE, id ) ) break;
				ref_ang = p( YP_PARAM_INDEX_FALL_ANGLE, id );
			}

			ref_ang *= index_ratio;

			xp->wang[id] = round( (xp->wang[id] - ref_ang - ang_diff) / ( 2.0 * M_PI * index_ratio ) ) * 
				2.0 * M_PI * index_ratio + ref_ang + ang_diff;
		}
		break;
	default:
		yprintf( OUTPUT_LV_ERROR, "Error: Unknown interrput data (%d, %d, %d)\n", param, id, value );
		break;
	}
}

/* Odometry型データの座標系を変換 */
void cstrans_odometry( YPSpur_cs cs, OdometryPtr dst_odm )
{
	double x, y, theta;
	x = g_odometry.x;
	y = g_odometry.y;
	theta = g_odometry.theta;

	CS_recursive_trans( get_cs_pointer( cs ), g_BS, &x, &y, &theta );

	dst_odm->x = x;
	dst_odm->y = y;
	dst_odm->theta = theta;
	dst_odm->time = g_odometry.time;
}

/* オドメトリへのポインタを取得 */
OdometryPtr get_odometry_ptr(  )
{
	return &g_odometry;
}

/**
 * @brief 時刻の推定 (n回目の計測結果の時刻を計算する)
 * @param int readnum[in] : 計測回数
 */
double time_estimate( int readnum )
{
	return g_offset + g_interval * ( double )( readnum - g_offset_point ) + g_estimated_delay;
}

/**
 * @brief ビットレートとか読み込める文字数を観測時刻の推定
 * @param receive_time[in] 最後にシリアルからデータを受け取った時間
 * @param readnum[in] オドメトリなどのデータセットを受け取った回数
 * @param wp[in] データ復元をできなかった残り文字数
 */
double time_synchronize( double receive_time, int readnum, int wp )
{
	static double prev_time = 0.0;
	static int prev_readnum = 0;
	double measured_time;

	// 受信開始時刻を計算
	if ( SER_BAUDRATE != 0 ) {
		measured_time = receive_time - ( wp / ( SER_BAUDRATE / 8.0 ) );
	}
	else {
		measured_time = receive_time;
	}
	// Add half USB FS frame delay
	// (Currently all YP-Spur compatible devices uses USB as a communication interface.)
	measured_time -= 0.0005;

	if( g_offset_point <= 0 || fabs(g_offset - measured_time) > 0.5 ) {
		// Reset if measured_time has too large error (500ms).
		g_offset = measured_time;
		g_interval = SER_INTERVAL;
		g_offset_point = readnum;
		prev_time = measured_time;
	}

	// predict
	g_offset += g_interval * (readnum - g_offset_point);
	g_offset_point = readnum;

	const double dt = measured_time - prev_time;
	double error = measured_time - g_offset;

	const int lost = lround(error / g_interval);
	if( lost != 0 ) {
		if( option( OPTION_SHOW_TIMESTAMP ) )
			printf("%d packet(s) might be lost!\n", lost);
		error -= lost * g_interval;
		g_offset_point -= lost;
	}

	static double error_integ = 0;
	error_integ += error * dt;

	if( error < g_estimated_delay && lost == 0 ) {
		if( option( OPTION_SHOW_TIMESTAMP ) )
			printf("[update min_delay] delay: %0.3f\n",
					error * 1000.0);
		g_estimated_delay = g_estimated_delay * 0.5 + error * 0.5;
	}
	g_estimated_delay *= (1.0 - dt / 120.0);

	g_offset += error * 0.2 + error_integ * 0.1;

	// aprox. 0.5 sec exp filter
	g_interval =
		0.99 * g_interval +
		0.01 * (( measured_time - prev_time ) / (double)( readnum - prev_readnum ));

	static int cnt_show_timestamp = 0;
	if( option( OPTION_SHOW_TIMESTAMP ) && ++cnt_show_timestamp % 100 == 0 )
		printf( "epoch: %0.8f, interval: %0.3f, delay: %0.3f, min_delay: %0.3f\n",
				g_offset, g_interval * 1000.0, error * 1000.0, g_estimated_delay * 1000.0 );

	prev_time = measured_time;
	prev_readnum = readnum;

	return g_offset;
}

/* シリアル受信処理 */
int odometry_receive( char *buf, int len, double receive_time, void *data )
{
	static int com_wp = 0;
	static int receive_count = 0;
	static char com_buf[128];
	static enum {
		ISOCHRONOUS = 0,
		INTERRUPT
	} mode = 0;

	int i;
	int odometry_updated;
	int readdata_num;
	int decoded_len = 0;
	int decoded_len_req; // Expected length of the data
	int readdata_len = 0;

	// Odometry *odm;
	Odometry odm_log[100];
	Short_2Char cnt1_log[100];
	Short_2Char cnt2_log[100];
	Short_2Char pwm1_log[100];
	Short_2Char pwm2_log[100];
	int ad_log[100][8];
	Parameters *param;

	param = get_param_ptr();

	decoded_len_req =
	 (
		+ get_ad_num(  ) /* ad */
		+ get_dio_num(  ) /* dio */
		+ param->num_motor_enable * 2 /* cnt + pwm */
	 ) * 2 /* data cnt -> byte */;
	readdata_num = 0;
	odometry_updated = 0;
	// 読み込まれたデータを解析
	for ( i = 0; i < len; i++ )
	{
		if( buf[i] == COMMUNICATION_START_BYTE )
		{
			com_wp = 0;
			mode = ISOCHRONOUS;
		}
		else if( buf[i] == COMMUNICATION_INT_BYTE )
		{
			com_wp = 0;
			mode = INTERRUPT;
		}
		else if( buf[i] == COMMUNICATION_END_BYTE )
		{
			unsigned char data[48];

			/* デコード処理 */
			decoded_len = decord( ( unsigned char * )com_buf, com_wp, ( unsigned char * )data, 48 );
			if( (mode == ISOCHRONOUS && decoded_len != decoded_len_req) ||
					(mode == INTERRUPT && decoded_len != 6) )
			{
				com_buf[com_wp] = '\0';
				yprintf( OUTPUT_LV_WARNING, "Warn: Broken packet '%s' (%d bytes) received. (%d bytes expected)\n", com_buf, decoded_len, decoded_len_req );
				com_wp = 0;
				continue;
			}

			switch(mode)
			{
			case ISOCHRONOUS:
				{
					short cnt[YP_PARAM_MAX_MOTOR_NUM], pwm[YP_PARAM_MAX_MOTOR_NUM];
					int i, p = 0;
					for(i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i ++)
					{
						if(!param->motor_enable[i]) continue;
						Short_2Char val;
						val.byte[1] = data[p++];
						val.byte[0] = data[p++];
						cnt[i] = val.integer;
					}
					for(i = 0; i < YP_PARAM_MAX_MOTOR_NUM; i ++)
					{
						if(!param->motor_enable[i]) continue;
						Short_2Char val;
						val.byte[1] = data[p++];
						val.byte[0] = data[p++];
						pwm[i] = val.integer;
					}

					process_addata( &data[p], decoded_len - p );

					cnt1_log[readdata_num].integer = cnt[0];
					cnt2_log[readdata_num].integer = cnt[1];
					pwm1_log[readdata_num].integer = pwm[0];
					pwm2_log[readdata_num].integer = pwm[1];
					memcpy( ad_log[readdata_num], get_addataptr(  ), sizeof ( int ) * 8 );
					readdata_num++;

					if( state( YP_STATE_MOTOR ) && state( YP_STATE_VELOCITY ) && state( YP_STATE_BODY ) )
					{
						odometry( &g_odometry, cnt, pwm, g_interval, time_estimate(receive_count + readdata_num) );
						odm_log[odometry_updated] = g_odometry;
						odometry_updated++;
					}

					if( option( OPTION_SHOW_ODOMETRY ) )
						printf( "%f %f %f\n", g_odometry.x, g_odometry.y, g_odometry.theta );
				}
				break;
			case INTERRUPT:
				{
					char param, id;
					Int_4Char value;

					param = data[0];
					id = data[1];
					value.byte[3] = data[2];
					value.byte[2] = data[3];
					value.byte[1] = data[4];
					value.byte[0] = data[5];

					process_int( &g_odometry, param, id, value.integer );
				}
				break;
			}
			readdata_len = com_wp;
			com_wp = 0;
		}
		else
		{
			com_buf[com_wp] = buf[i];
			com_wp++;
			if( com_wp >= 128 )
			{
				com_wp = 128 - 1;
				yprintf( OUTPUT_LV_WARNING, "Warn: Read buffer overrun.\n" );
			}
		}
	}

	if ( readdata_num > 0 ) {
		receive_count += readdata_num;
		if( com_wp == 0 ) time_synchronize( receive_time, receive_count, com_wp );
	}

	write_ypspurSSM( odometry_updated, receive_count, odm_log, readdata_num, cnt1_log, cnt2_log, pwm1_log, pwm2_log,
					 ad_log );
	return 1;
}

int odometry_receive_loop( void )
{
	int ret;
	Parameters *param;
	param = get_param_ptr();

	g_interval = SER_INTERVAL;
	while( 1 )
	{
		ret = serial_recieve( odometry_receive, NULL );
		if( param->parameter_applying )
		{
			yprintf( OUTPUT_LV_MODULE, "Restarting odometry receive loop.\n" );
			continue;
		}
		break;
	}

	return ret;
}

