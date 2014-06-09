#ifndef __ADINPUT__
#define __ADINPUT__

/* high level I/O */
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <strings.h>

int process_addata( unsigned char *buf, int len );
const int *get_addataptr(  );
int get_addata( int num );
int admask_receive( char *buf, int len, double receive_time, void *data );
int set_admask( unsigned char mask );
int set_diomask( unsigned char enable );
int get_ad_num( void );
int get_dio_num( void );

#endif
