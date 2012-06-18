#ifndef __SSM_SPUR_HANDLER__
#define __SSM_SPUR_HANDLER__
#include "command.h"

void init_ypspurSSM( int );
void end_ypspurSSM( void );
void write_ypspurSSM( int odometry_updated, int receive_count,
					  Odometry * odm_log, int readdata_num, Short_2Char * cnt1_log, Short_2Char * cnt2_log,
					  Short_2Char * pwm1_log, Short_2Char * pwm2_log, int ad_log[][8] );
void coordinate_synchronize( Odometry * odm, SpurUserParamsPtr spur );

#endif
