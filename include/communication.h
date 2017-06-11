#ifndef __COMMUNICATION__
#define __COMMUNICATION__

#define COMMUNICATION_START_BYTE	0x09
#define COMMUNICATION_INT_BYTE		0x07
#define COMMUNICATION_END_BYTE		0x0a

int encode( unsigned char *src, int len, unsigned char *dst, int buf_max );
int decord( unsigned char *src, int len, unsigned char *dst, int buf_max );
int encode_write( char *data, int len );
#endif
