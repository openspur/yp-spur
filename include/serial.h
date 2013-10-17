#ifndef __SERIAL__
#define __SERIAL__

#define ENABLE 1
#define DISABLE 0

#define DEFAULT_BAUDRATE	38400

/* for measurement time estimation */
extern double SER_BAUDRATE; /// (Byte/sec) シリアルの通信速度
#define SER_INTERVAL 0.0050
// #define SER_BYTES 13.0
// #define SER_TIME_BYTE (11.0/SER_BAUDRATE) 

int serial_tryconnect( char *device_name );
int serial_connect( char *device_name );
int serial_change_baudrate( int baud );

/*----------------PBS_close------------------*/
int serial_close( void );
int serial_write( char *buf, int len );
int serial_recieve( int ( *serial_event ) ( char *, int, double, void * ), void *data );
void serial_flush_in( void );
void serial_flush_out( void );

#endif
