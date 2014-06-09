#ifndef __UTILITY__
#define __UTILITY__

double get_time( void );
void yp_usleep( int usec );
void set_sigint_handler( void (*handler)(int) );
void hook_pre_global( void );

#if !defined(HAVE_STRTOK_R)
#ifndef strtok_r
char* strtok_r( char *str, const char *delim, char **nextp );
#endif
#endif

#endif
