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
	g_odometry.x = 0;
	g_odometry.y = 0;
	g_odometry.theta = 0;
	g_odometry.theta_r = 0;
	g_odometry.theta_l = 0;
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
void odometry( OdometryPtr xp, short cnt1, short cnt2, short pwm1, short pwm2, double dt )
{
	double v, w, wr, wl, mwr, mwl;
	double mtorque_l, mtorque_r, torque_l, torque_r, volt_l, volt_r, vc_l, vc_r;
	double torque_trans, torque_angular;

	/* 角速度計算 */
	mwr = 2.0 * M_PI * ( ( double )cnt1 ) / ( p( YP_PARAM_COUNT_REV, MOTOR_RIGHT ) * dt );
	mwl = 2.0 * M_PI * ( ( double )cnt2 ) / ( p( YP_PARAM_COUNT_REV, MOTOR_LEFT ) * dt );
	wr = mwr / p( YP_PARAM_GEAR, MOTOR_RIGHT );
	wl = mwl / p( YP_PARAM_GEAR, MOTOR_LEFT );
	/* キネマティクス計算 */
	v = p( YP_PARAM_RADIUS, MOTOR_RIGHT ) * wr / 2.0 + p( YP_PARAM_RADIUS, MOTOR_LEFT ) * wl / 2.0;
	w = +p( YP_PARAM_RADIUS, MOTOR_RIGHT ) * wr / p( YP_PARAM_TREAD, 0 )
	    -p( YP_PARAM_RADIUS, MOTOR_LEFT ) * wl / p( YP_PARAM_TREAD, 0 );

	volt_r = ( double )pwm1 * p( YP_PARAM_VOLT, MOTOR_RIGHT )
			 / ( p( YP_PARAM_PWM_MAX, MOTOR_RIGHT ) * ( dt / p( YP_PARAM_CYCLE, MOTOR_RIGHT ) ) );
	volt_l = ( double )pwm2 * p( YP_PARAM_VOLT, MOTOR_LEFT )
			 / ( p( YP_PARAM_PWM_MAX, MOTOR_LEFT ) * ( dt / p( YP_PARAM_CYCLE, MOTOR_LEFT ) ) );

	/* トルク推定 */
	vc_r = ( p( YP_PARAM_MOTOR_VC, MOTOR_RIGHT ) / 60 ) * 2 * M_PI; // [rpm/V] => [(rad/s)/V]
	vc_l = ( p( YP_PARAM_MOTOR_VC, MOTOR_LEFT ) / 60 ) * 2 * M_PI; // [rpm/V] => [(rad/s)/V]
	// TC [Nm/A]
	mtorque_r = p( YP_PARAM_MOTOR_TC, MOTOR_RIGHT ) * ( ( volt_r - mwr / vc_r ) / p( YP_PARAM_MOTOR_R, MOTOR_RIGHT ) );
	mtorque_l = p( YP_PARAM_MOTOR_TC, MOTOR_LEFT ) * ( ( volt_l - mwl / vc_l ) / p( YP_PARAM_MOTOR_R, MOTOR_LEFT ) );
	/* 摩擦補償の補償 */
	if( wr > 0 )
	{
		mtorque_r -= p( YP_PARAM_TORQUE_NEWTON, MOTOR_RIGHT )
		           + p( YP_PARAM_TORQUE_VISCOS, MOTOR_RIGHT ) * fabs( mwr );
	}
	else if( wr < 0 )
	{
		mtorque_r += p( YP_PARAM_TORQUE_NEWTON_NEG, MOTOR_RIGHT )
		           + p( YP_PARAM_TORQUE_VISCOS_NEG, MOTOR_RIGHT ) * fabs( mwr );
	}
	if( wl > 0 )
	{
		mtorque_l -= p( YP_PARAM_TORQUE_NEWTON, MOTOR_LEFT )
		           + p( YP_PARAM_TORQUE_VISCOS, MOTOR_LEFT ) * fabs( mwl );
	}
	else if( wl < 0 )
	{
		mtorque_l += p( YP_PARAM_TORQUE_NEWTON_NEG, MOTOR_LEFT )
		           + p( YP_PARAM_TORQUE_VISCOS_NEG, MOTOR_LEFT ) * fabs( mwl );
	}
	
	//printf("%+f %+f     %+f %+f   %+f %+f\n",wr,wl,mtorque_r,mtorque_l,volt_r,volt_l);

	torque_r = mtorque_r * p( YP_PARAM_GEAR, MOTOR_RIGHT );
	torque_l = mtorque_l * p( YP_PARAM_GEAR, MOTOR_LEFT );

	torque_trans = torque_r / p( YP_PARAM_RADIUS, MOTOR_RIGHT )
	               + torque_l / p( YP_PARAM_RADIUS, MOTOR_LEFT );
	torque_angular = ( + torque_r / p( YP_PARAM_RADIUS, MOTOR_RIGHT )
					   - torque_l / p( YP_PARAM_RADIUS, MOTOR_LEFT ) )
	                   * p( YP_PARAM_TREAD, 0 ) / 2;

	/* オドメトリ計算 */
	xp->x = xp->x + v * cos( xp->theta ) * dt;
	xp->y = xp->y + v * sin( xp->theta ) * dt;
	xp->theta = xp->theta + w * dt;
	xp->time = get_time(  );
	xp->v = v;
	xp->w = w;
	xp->wr = wr;
	xp->wl = wl;
	xp->theta_r = xp->theta_r + xp->wr * dt;
	xp->theta_l = xp->theta_l + xp->wl * dt;
	xp->torque_r = torque_r;
	xp->torque_l = torque_l;
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
	return g_offset + g_interval * ( double )( readnum - g_offset_point );
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
	static double minsub = 0;
	static double minsub_time = 0;
	static int minsub_readnum = 0;
	static int prev_readnum = 0;
	double estimated_time;
	double measured_time;
	double sub;

	// 受信開始時刻を計算
	if ( SER_BAUDRATE != 0 ) {
		measured_time = receive_time;
	}
	else {
		measured_time = receive_time - ( wp / ( SER_BAUDRATE / 8.0 ) );
	}
	
	// 初回の場合 
	if( g_offset_point <= 0 ) {
		// 初回のデータを基準として保存
		g_offset = measured_time;
		g_interval = SER_INTERVAL;
		g_offset_point = readnum;

		// 更新時のデータを保存
		prev_time = measured_time;
		prev_readnum = readnum;

		// 推定値に対して最も早い受信時刻のデータを初期化
		minsub_time = measured_time;
		minsub_readnum = readnum;
		// 適当な大きな値(インターバル以上遅れると異常)
		minsub = g_interval;
		
		// オドメトリの更新回数からの推定値を計算
		return measured_time;
	}
	else {
		// オドメトリの更新回数からの推定値を計算
		estimated_time = time_estimate(readnum);

		// オドメトリの更新回数からの推定値より早いデータほど計測時刻に近いと期待
		sub = measured_time - estimated_time;
		if( minsub > sub) {
			// 最も早い受信時刻を保存
			minsub = sub;
			minsub_time = measured_time;
			minsub_readnum = readnum;
		}

		// 十分に時間が経過している場合
		if( receive_time - prev_time > 0.5 ) {
			// 基準のデータを更新			
			g_offset = minsub_time;
			g_offset_point = minsub_readnum;

			// オドメトリの更新間隔を推定
			g_interval = (measured_time - prev_time) / (double)( readnum - prev_readnum );

			if( option( OPTION_SHOW_TIMESTAMP ) )
				printf( "%f %f \n", g_offset, g_interval * 1000.0 );

			// 更新時のデータを保存
			prev_time = measured_time;
			prev_readnum = readnum;

			// 推定値に対して最も早い受信時刻のデータを初期化
			minsub_time = measured_time;
			minsub_readnum = readnum;
			minsub = sub;
		}

		return estimated_time;
	}
}

/* シリアル受信処理 */
int odometry_receive( char *buf, int len, double receive_time, void *data )
{
	static int com_wp = 0;
	static int receive_count = 0;
	static char com_buf[128];

	int i;
	int odometry_updated;
	int readdata_num;
	int decoded_len = 0;
	int decoded_len_success; // デコードした際の、正しいバイト数
	int readdata_len = 0;

	// Odometry *odm;
	Odometry odm_log[100];
	Short_2Char cnt1_log[100];
	Short_2Char cnt2_log[100];
	Short_2Char pwm1_log[100];
	Short_2Char pwm2_log[100];
	int ad_log[100][8];

	decoded_len_success =
	 (
		4 /* encorder cnt + pwm */
		+ get_ad_num(  ) /* ad */
		+ get_dio_num(  ) /* dio */
	 ) * 2 /* data cnt -> byte */;
	readdata_num = 0;
	odometry_updated = 0;
	// 読み込まれたデータを解析
	for ( i = 0; i < len; i++ )
	{
		if( buf[i] == COMMUNICATION_START_BYTE )
		{
			com_wp = 0;
		}
		else if( buf[i] == COMMUNICATION_END_BYTE )
		{
			unsigned char data[48];
			Short_2Char cnt1, cnt2, pwm1, pwm2 /* , ad[8] */ ;
			// int j, ad_num;

			/* デコード処理 */
			decoded_len = decord( ( unsigned char * )com_buf, com_wp, ( unsigned char * )data, 48 );
			if( decoded_len != decoded_len_success )
			{
				com_buf[com_wp] = '\0';
				yprintf( OUTPUT_LV_WARNING, "Warn: Illegal packet '%s' received.\n", com_buf );
				com_wp = 0;
				continue;
			}
			cnt1.byte[1] = data[0];
			cnt1.byte[0] = data[1];
			cnt2.byte[1] = data[2];
			cnt2.byte[0] = data[3];
			pwm1.byte[1] = data[4];
			pwm1.byte[0] = data[5];
			pwm2.byte[1] = data[6];
			pwm2.byte[0] = data[7];

			process_addata( &data[8], decoded_len - 8 );

			cnt1_log[readdata_num] = cnt1;
			cnt2_log[readdata_num] = cnt2;
			pwm1_log[readdata_num] = pwm1;
			pwm2_log[readdata_num] = pwm2;
			memcpy( ad_log[readdata_num], get_addataptr(  ), sizeof ( int ) * 8 );

			if( state( YP_STATE_MOTOR ) && state( YP_STATE_VELOCITY ) && state( YP_STATE_BODY ) )
			{
				odometry( &g_odometry, cnt1.integer, cnt2.integer, pwm1.integer, pwm2.integer, g_interval );
				odm_log[odometry_updated] = g_odometry;
				odometry_updated++;
			}

			if( option( OPTION_SHOW_ODOMETRY ) )
				printf( "%f %f %f\n", g_odometry.x, g_odometry.y, g_odometry.theta );
		
			readdata_num++;
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
				yprintf( OUTPUT_LV_WARNING, "Warn: Illegal data received.\n" );
			}
		}
	}

	if ( readdata_num > 0 ) {
		receive_count += readdata_num;
		time_synchronize( receive_time, receive_count, readdata_len + com_wp );
	}

	write_ypspurSSM( odometry_updated, receive_count, odm_log, readdata_num, cnt1_log, cnt2_log, pwm1_log, pwm2_log,
					 ad_log );
	return 1;
}

int odometry_receive_loop( void )
{
	g_interval = SER_INTERVAL;
	return serial_recieve( odometry_receive, NULL );
}

