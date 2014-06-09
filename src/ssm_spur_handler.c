/* high level I/O */
#include <stdio.h>
#include <string.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* SSM 用 */
#ifdef HAVE_SSM
#include <ssm.h>
#include "ssmtype/spur-odometry.h"
#include "ssmtype/pws-motor.h"
#include "ssmtype/ypspur-ad.h"
#endif

/* yp-spur用 */
#include "odometry.h"
#include "utility.h"
#include "ypspur.h"
#include "param.h"
#include "shvel-param.h"
#include "ssm_spur_handler.h"

#ifdef HAVE_SSM
SSM_sid g_odm_bs_sid = 0, g_odm_sid = 0, g_motor_sid = 0, g_odm_adj_sid = 0, g_ad_sid = 0;
int g_ssm_enable;
int g_ssm_adj_enable = 0;
int g_ssm_id = 0;
#endif

/* SSM の初期化 */
void init_ypspurSSM( int ssm_id )
{
#ifdef HAVE_SSM
	g_ssm_id = ssm_id;

	yprintf( OUTPUT_LV_MODULE, "    with SSM\n" );
	if( !initSSM(  ) )
	{
		/* SSMが起動していない */
		yprintf( OUTPUT_LV_ERROR, "\n SSM is not available.\n" );
		g_ssm_enable = 0;
	}
	else
	{
		g_ssm_enable = 1;
		g_odm_bs_sid = createSSM_time( SNAME_ODOMETRY, ssm_id, sizeof ( Spur_Odometry ), 5, 0.005 );
		g_odm_sid = createSSM_time( SNAME_GLOBAL, ssm_id, sizeof ( Spur_Odometry ), 5, 0.005 );
		g_motor_sid = createSSM_time( SNAME_PWS_MOTOR, ssm_id, sizeof ( PWSMotor ), 1, 0.005 );
		g_ad_sid = createSSM_time( SNAME_YPSPUR_AD, ssm_id, sizeof ( YP_ad ), 1, 0.005 );
		if( !( g_odm_bs_sid && g_odm_sid && g_motor_sid && g_ad_sid ) )
		{
			g_ssm_enable = 0;
			yprintf( OUTPUT_LV_WARNING, "\nWarning : cannot create ssm.\n" );
		}
	}
	g_ssm_adj_enable = 0;
#endif
}

void end_ypspurSSM(  )
{
#ifdef HAVE_SSM
	if( g_ssm_enable )
	{
		releaseSSM( &g_odm_bs_sid );
		releaseSSM( &g_odm_sid );
		releaseSSM( &g_motor_sid );
		releaseSSM( &g_ad_sid );
		releaseSSM( &g_odm_adj_sid );
		endSSM(  );
	}
#endif
}

void write_ypspurSSM( int odometry_updated, int receive_count,
					  Odometry * odm_log, int readdata_num, Short_2Char * cnt1_log, Short_2Char * cnt2_log,
					  Short_2Char * pwm1_log, Short_2Char * pwm2_log, int ad_log[][8] )
{
#ifdef HAVE_SSM
	if( g_ssm_enable )
	{
		int i;
		PWSMotor motor;
		Spur_Odometry odm;
		YP_ad ad;
		ssmTimeT time;
		int time_offset;
		/* SSMへの出力 */
		time_offset = receive_count - odometry_updated + 1;
		for ( i = 0; i < odometry_updated; i++ )
		{
			time = time_estimate( time_offset + i );
			/* BSの出力 */
			odm.x = odm_log[i].x;
			odm.y = odm_log[i].y;
			odm.theta = odm_log[i].theta;
			odm.v = odm_log[i].v;
			odm.w = odm_log[i].w;
			writeSSM( g_odm_bs_sid, &odm, time );
			/* GLの出力 */
			CS_recursive_trans( get_cs_pointer( CS_GL ), get_cs_pointer( CS_BS ), &odm.x, &odm.y, &odm.theta );
			writeSSM( g_odm_sid, &odm, time );
		}
		/* PWM,カウンタ値,AD値の出力 */
		time_offset = receive_count - readdata_num + 1;
		for ( i = 0; i < readdata_num; i++ )
		{
			time = time_estimate( time_offset + i );
			motor.counter1 = cnt1_log[i].integer;
			motor.counter2 = cnt2_log[i].integer;
			motor.pwm1 = pwm1_log[i].integer;
			motor.pwm2 = pwm2_log[i].integer;
			memcpy( ad.ad, ad_log[i], sizeof ( int ) * 8 );
			writeSSM( g_motor_sid, &motor, time );
			writeSSM( g_ad_sid, &ad, time );
		}
	}
#endif
}

/* オドメトリ修正情報との融合 */
void coordinate_synchronize( Odometry * odm, SpurUserParamsPtr spur )
{
#ifdef HAVE_SSM
	static double before_time;
	double now_time, time;
	Odometry bs_odometry, adj_odometry, target_pos;
	CoordinateSystem bs_cs, adj_cs;
	int tid;
	if( g_ssm_enable )
	{											/* 20ms毎？ */
		if( !g_ssm_adj_enable )
		{										/* SSMcheck */
			now_time = get_time(  );
			if( now_time > before_time + 1 )
			{
				g_odm_adj_sid = openSSM( SNAME_ADJUST, 0, 0 );
				if( g_odm_adj_sid > 0 )
				{								// openできた
					g_ssm_adj_enable = 1;
					yprintf( OUTPUT_LV_MODULE, "SSMInfo: Find adjust information.\n" );
				}
				else
				{
					g_ssm_adj_enable = 0;
				}
				before_time = now_time;
			}
		}
		else
		{
			/* パラメータの変更がおこらないようにブロック */
			pthread_mutex_lock( &spur->mutex );
			// 最新の修正位置
			if( ( tid = readSSM( g_odm_adj_sid, ( char * )&adj_odometry, &now_time, -1 ) ) >= 0 )
			{
				// 同時刻のGL座標
				if( now_time > get_time(  ) - 1 )
				{
					if( ( tid = readSSM_time( g_odm_bs_sid, ( char * )&bs_odometry, now_time, &time ) ) >= 0 )
					{
						// 時間が1秒以内（止まっていない）で、データがあるなら実行
						/* 座標系作成 */
						bs_cs.x = bs_odometry.x;
						bs_cs.y = bs_odometry.y;
						bs_cs.theta = bs_odometry.theta;
						adj_cs.x = adj_odometry.x;
						adj_cs.y = adj_odometry.y;
						adj_cs.theta = adj_odometry.theta;
						/* 修正後の位置を計算 */
						target_pos = *odm;		/* 最新の位置 */
						/* 現在位置との微妙な差を計算 */
						trans_cs( &bs_cs, &target_pos.x, &target_pos.y, &target_pos.theta );
						/* 微妙な差を付け加える */
						inv_trans_cs( &adj_cs, &target_pos.x, &target_pos.y, &target_pos.theta );
						/* ロボット(FS)がGL上のx,y,thetaに見えるとするとき */
#if 0
						/* FSからGLがどこに見えるか(GL->FS => FS->GL) */
						CS_turn_base( &target_pos.x, &target_pos.y, &target_pos.theta );
						/* それはBS上のどこか */
						trans_cs( get_cs_pointer( CS_FS ), get_cs_pointer( CS_BS ), &target_pos.x, &target_pos.y,
								  &target_pos.theta );
						/* GLをセット */
						set_cs( CS_GL, target_pos.x, target_pos.y, target_pos.theta );
#else
						double data[3] = { target_pos.x, target_pos.y, target_pos.theta };
						set_adjust_com( CS_GL, data, spur );
#endif
					}
				}
			}
			/* 保護終わり */
			pthread_mutex_unlock( &spur->mutex );
		}
	}
#endif
}
