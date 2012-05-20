#ifndef __UTILITY__
#define __UTILITY__

double get_time( void );
void yp_usleep( int usec );
void set_sigint_handler( void (*handler)(int) );

#endif
