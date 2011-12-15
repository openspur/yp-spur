
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#if HAVE_LIBREADLINE
#	include <readline/readline.h>
#	include <readline/history.h>
#endif

#include <ypspur.h>
#include <utility.h>


typedef struct SPUR_COMMAND
{
	enum
	{
		SPUR_NONE = -1,
		SPUR_LINE,
		SPUR_STOP_LINE,
		SPUR_CIRCLE,
		SPUR_SPIN,
		SPUR_GETPOS,
		SPUR_GETVEL,
		SPUR_SETPOS,
		SPUR_GETFORCE,
		SPUR_NEAR_POS,
		SPUR_NEAR_ANG,
		SPUR_OVER_LINE,
		SPUR_ADJUSTPOS,
		SPUR_SETVEL,
		SPUR_SETANGVEL,
		SPUR_SETACCEL,
		SPUR_SETANGACCEL,
		SPUR_INIT,
		SPUR_FREE,
		SPUR_STOP,
		SPUR_FREEZE,
		SPUR_UNFREEZE,
		SPUR_ISFREEZE,
		SPUR_GETAD,
		SPUR_VEL,
		SPUR_WHEEL_VEL,
		SPUR_GET_WHEEL_VEL,
		SPUR_GET_WHEEL_ANG,
		SPUR_GET_WHEEL_TORQUE,
		SPUR_WHEEL_TORQUE,
		SPUR_ORIENT,
		EXIT,
		HELP,
		SPUR_COMMAND_MAX
	} id;
	char name[64];
	int reqarg;
	int narg;
	double arg[3];
} SpurCommand;

static const SpurCommand SPUR_COMMAND[SPUR_COMMAND_MAX] = {
	{SPUR_LINE, {"line"}, 3},
	{SPUR_STOP_LINE, {"stop_line"}, 3},
	{SPUR_CIRCLE, {"circle"}, 3},
	{SPUR_SPIN, {"spin"}, 1},
	{SPUR_GETPOS, {"get_pos"}, 0},
	{SPUR_GETVEL, {"get_vel"}, 0},
	{SPUR_GETFORCE, {"get_force"}, 0},
	{SPUR_SETPOS, {"set_pos"}, 3},
	{SPUR_NEAR_POS, {"near_pos"}, 3},
	{SPUR_NEAR_ANG, {"near_ang"}, 2},
	{SPUR_OVER_LINE, {"over_line"}, 3},
	{SPUR_ADJUSTPOS, {"adjust_pos"}, 3},
	{SPUR_SETVEL, {"set_vel"}, 1},
	{SPUR_SETANGVEL, {"set_angvel"}, 1},
	{SPUR_SETACCEL, {"set_accel"}, 1},
	{SPUR_SETANGACCEL, {"set_angaccel"}, 1},
	{SPUR_FREE, {"free"}, 0},
	{SPUR_STOP, {"stop"}, 0},
	{SPUR_INIT, {"init"}, 0},
	{SPUR_FREEZE, {"freeze"}, 0},
	{SPUR_UNFREEZE, {"unfreeze"}, 0},
	{SPUR_ISFREEZE, {"isfreeze"}, 0},
	{SPUR_GETAD, {"get_ad_value"}, 1},
	{SPUR_VEL, {"vel"}, 2},
	{SPUR_WHEEL_VEL, {"wheel_vel"}, 2},
	{SPUR_GET_WHEEL_VEL, {"get_wheel_vel"}, 0},
	{SPUR_GET_WHEEL_ANG, {"get_wheel_ang"}, 0},
	{SPUR_GET_WHEEL_TORQUE, {"get_wheel_torque"}, 0},
	{SPUR_WHEEL_TORQUE, {"wheel_torque"}, 2},
	{SPUR_ORIENT, {"orient"}, 1},
	{HELP, {"help"}, 0},
	{EXIT, {"exit"}, 0}
};

#if HAVE_SIGLONGJMP
sigjmp_buf ctrlc_capture;
#endif

void ctrlc( int num )
{
#if HAVE_SIGLONGJMP
	siglongjmp( ctrlc_capture, 1 );
#endif
}

int main( void )
{
	int coordinate = CS_FS;
	int active = 1;
	int ad;
	int err = 0;

#if HAVE_SIGLONGJMP
	signal( SIGINT, ctrlc );
#endif

	YPSpur_init(  );

#if HAVE_LIBREADLINE
	using_history(  );
	read_history( ".spurip_history" );
#endif
	while( active )
	{
		static char *line = NULL;
		static char *line_prev = NULL;
		char *argv;
		char text[16];
		int i;
#if !HAVE_LIBREADLINE
#	if HAVE_GETLINE
		size_t len;
#	endif
#endif
		int ret;
		SpurCommand spur;
		int lcoordinate;
		enum
		{
			MODE_COMMAND,
			MODE_ARG
		} mode = MODE_COMMAND;

#if HAVE_SIGLONGJMP
		if( sigsetjmp( ctrlc_capture, 1 ) != 0 )
		{
			printf( "\n" );
		}
		else
#endif
		{
			{
				// Dummy for error checking
				double x, y, th;
				YPSpur_get_pos( CS_GL, &x, &y, &th );
			}
			if( YP_get_error_state() ){
				Spur_init();
				if( err == 0 ){
					fprintf( stderr, "WARN: YPSpur-coordinator terminated.\n" );
					fflush( stderr );
#if HAVE_SIGLONGJMP
					signal( SIGINT, NULL );
#endif
				}
				err = 1;
				yp_usleep( 50000 );
				continue;
			}else{
				if( err == 1 ){
					fprintf( stderr, "INFO: YPSpur-coordinator started.\n" );
					fflush( stderr );
#if HAVE_SIGLONGJMP
					signal( SIGINT, ctrlc );
#endif
				}
			}
			err = 0;


			sprintf( text, "%s> ", YPSpur_CSName[coordinate] );
#if HAVE_LIBREADLINE
			line_prev = line;
			line = readline( text );
			if( strlen( line ) > 0 )
			{
				if( line && line_prev )
				{
					if( strcmp( line, line_prev ) != 0 )
					{
						add_history( line );
					}
				}
				else
				{
					add_history( line );
				}
			}
#else
			printf( "%s", text );
			fflush( stdout );
			line = NULL;
#	if HAVE_GETLINE
			len = 0;
			getline( &line, &len, stdin );
			if( len == 0 ) continue;
#	else
			line = malloc( 512 );
			fgets( line, 512, stdin );
#	endif
			line_prev = line;
#endif

			spur.id = -1;
			lcoordinate = coordinate;

			argv = strtok( line, " \t\n\r" );
			if( !argv )
			{
				continue;
			}

			while( argv )
			{
				switch ( mode )
				{
				case MODE_COMMAND:
					for ( i = 0; i < CS_MAX; i++ )
					{
						if( strcmp( argv, YPSpur_CSName[i] ) == 0 )
						{
							lcoordinate = i;
							break;
						}
					}
					if( i != CS_MAX )
					{
						break;
					}
					for ( i = 0; i < SPUR_COMMAND_MAX; i++ )
					{
						if( strcmp( argv, SPUR_COMMAND[i].name ) == 0 )
						{
							spur = SPUR_COMMAND[i];
							break;
						}
					}
					mode = MODE_ARG;
					break;
				default:
					spur.arg[mode - MODE_ARG] = atof( argv );
					mode++;
					spur.narg = mode - MODE_ARG;
					if( mode > MODE_ARG + 3 )
						break;
					break;
				}
				argv = strtok( NULL, " \t\n\r" );
			}

			if( line_prev ) free( line_prev );
			if( spur.id < 0 )
			{
				coordinate = lcoordinate;
				continue;
			}

			if( spur.narg < spur.reqarg )
			{
				fprintf( stderr, "error: too few arguments to %s\n", spur.name );
				fflush( stderr );
				continue;
			}

			switch ( spur.id )
			{
			case SPUR_LINE:
				YPSpur_line( lcoordinate, spur.arg[0], spur.arg[1], spur.arg[2] );
				break;
			case SPUR_STOP_LINE:
				YPSpur_stop_line( lcoordinate, spur.arg[0], spur.arg[1], spur.arg[2] );
				break;
			case SPUR_CIRCLE:
				YPSpur_circle( lcoordinate, spur.arg[0], spur.arg[1], spur.arg[2] );
				break;
			case SPUR_ORIENT:
				YPSpur_orient( lcoordinate, spur.arg[0] );
				break;
			case SPUR_SPIN:
				YPSpur_spin( lcoordinate, spur.arg[0] );
				break;
			case SPUR_GETPOS:
				YPSpur_get_pos( lcoordinate, &spur.arg[0], &spur.arg[1], &spur.arg[2] );
				printf( "%f %f %f\n", spur.arg[0], spur.arg[1], spur.arg[2] );
				break;
			case SPUR_GETVEL:
				YPSpur_get_vel( &spur.arg[0], &spur.arg[1] );
				printf( "%f %f\n", spur.arg[0], spur.arg[1] );
				break;
			case SPUR_GETFORCE:
				YPSpur_get_force( &spur.arg[0], &spur.arg[1] );
				printf( "%f %f\n", spur.arg[0], spur.arg[1] );
				break;
			case SPUR_GET_WHEEL_TORQUE:
				YP_get_wheel_torque( &spur.arg[0], &spur.arg[1] );
				printf( "%f %f\n", spur.arg[0], spur.arg[1] );
				break;
			case SPUR_SETPOS:
				YPSpur_set_pos( lcoordinate, spur.arg[0], spur.arg[1], spur.arg[2] );
				break;
			case SPUR_NEAR_POS:
				ret = YPSpur_near_pos( lcoordinate, spur.arg[0], spur.arg[1], spur.arg[2] );
				printf( "%d\n", ret );
				break;
			case SPUR_NEAR_ANG:
				ret = YPSpur_near_ang( lcoordinate, spur.arg[0], spur.arg[1] );
				printf( "%d\n", ret );
				break;
			case SPUR_OVER_LINE:
				ret = YPSpur_over_line( lcoordinate, spur.arg[0], spur.arg[1], spur.arg[2] );
				printf( "%d\n", ret );
				break;
			case SPUR_ADJUSTPOS:
				YPSpur_adjust_pos( lcoordinate, spur.arg[0], spur.arg[1], spur.arg[2] );
				break;
			case SPUR_SETVEL:
				YPSpur_set_vel( spur.arg[0] );
				break;
			case SPUR_SETANGVEL:
				YPSpur_set_angvel( spur.arg[0] );
				break;
			case SPUR_SETACCEL:
				YPSpur_set_accel( spur.arg[0] );
				break;
			case SPUR_SETANGACCEL:
				YPSpur_set_angaccel( spur.arg[0] );
				break;
			case SPUR_STOP:
				YPSpur_stop(  );
				break;
			case SPUR_INIT:
				YPSpur_init(  );
				break;
			case SPUR_FREE:
				YPSpur_free(  );
				break;
			case SPUR_FREEZE:
				YPSpur_freeze(  );
				break;
			case SPUR_UNFREEZE:
				YPSpur_unfreeze(  );
				break;
			case SPUR_ISFREEZE:
				ret = YPSpur_isfreeze(  );
				printf( "%d\n", ret );
				break;
			case SPUR_GETAD:
				ad = YP_get_ad_value( ( int )spur.arg[0] );
				printf( "%d\n", ad );
				break;
			case SPUR_VEL:
				YPSpur_vel( spur.arg[0], spur.arg[1] );
				break;
			case SPUR_WHEEL_VEL:
				YP_set_wheel_vel( spur.arg[0], spur.arg[1] );
				break;
			case SPUR_WHEEL_TORQUE:
				YP_set_wheel_torque( spur.arg[0], spur.arg[1] );
				break;
			case SPUR_GET_WHEEL_VEL:
				YP_get_wheel_vel( &spur.arg[0], &spur.arg[1] );
				printf( "%f %f\n", spur.arg[0], spur.arg[1] );
				break;
			case SPUR_GET_WHEEL_ANG:
				YP_get_wheel_ang( &spur.arg[0], &spur.arg[1] );
				printf( "%f %f\n", spur.arg[0], spur.arg[1] );
				break;
			case HELP:
				printf( "Usage:\n" );
				printf( "   > command arg1 arg2 arg3\n" );
				printf( "   > coordinate\n" );
				printf( "   > coordinate command arg1 arg2 arg3\n" );
				printf( "Commands:\n" );
				for ( i = 0; i < SPUR_COMMAND_MAX; i++ )
				{
					printf( "   %s\n", SPUR_COMMAND[i].name );
				}
				printf( "Coordinates:\n" );
				for ( i = 0; i < CS_MAX + 1; i++ )
				{
					printf( "   %s\n", YPSpur_CSName[i] );
				}
				break;
			case EXIT:
				active = 0;
				break;
			default:
				printf( "unknown command\n" );
			}
			fflush( stdout );
		}
	}
#if HAVE_LIBREADLINE
	write_history( ".spurip_history" );
#endif

	return 0;
}
