#ifndef __SH_VEL__
#define __SH_VEL__

#define LOCK 1
#define UNLOCK 0

void init( void );
void initServo( void );
int command_analyze( char *data, int len );
int extended_command_analyze( int channel, char *data );


/* Driver parameters */
#define YP_DRIVERPARAM_PWMRES "1000"

#endif
