#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <ypspur.h>
#include <utility.h>

int main( int argc, char **argv )
{
	hook_pre_global();
	if( argc > 1 )
	{
		char ip[64];
		char *p;
		int port;

		strncpy( ip, argv[1], 64 );
		p = strchr( ip, ':' );
		if( p == NULL )
		{
			fprintf( stderr, "USAGE: %s ip:port\n", argv[0] );
			return -1;
		}
		*p = 0;
		port = atoi( p + 1 );
		Spur_init_socket( ip, port );
	}
	else
	{
		if( Spur_init(  ) < 0 )
		{
			fprintf( stderr, "ERROR: ypspur-coordinator stopped.\n" );
			return -1;
		}
	}

	Spur_free(  );

	return 0;
}
