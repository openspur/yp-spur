#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <ypspur.h>

#ifdef __WIN32
#	include <windows.h>
#endif

int main( int argc, char *argv[] )
{
	double x, y, theta;
	
	// Windows環境で標準出力がバッファリングされないように設定
	setvbuf( stdout, 0, _IONBF, 0 );
	
	// 初期化
	if(	Spur_init(  ) < 0 )
	{
		fprintf(stderr, "ERROR : cannot open spur.\n");
		return -1;
	}

	Spur_set_vel( 0.2 );
	Spur_set_accel( 1.0 );
	Spur_set_angvel( M_PI / 2.0 );
	Spur_set_angaccel( M_PI / 2.0 );

	Spur_set_pos_GL( 0, 0, 0 );
	printf( "line\n" );
	// 走行
	Spur_stop_line_GL( 1.0, 0, 0 );
	while( !Spur_over_line_GL( 1.0 - 0.005, 0.0, 0.0 ) )
		usleep( 100000 );

	// 回転
	printf( "spin\n" );
	Spur_spin_GL( M_PI / 2 );
	while( !Spur_near_ang_GL( M_PI / 2, M_PI / 18.0 ) )
		usleep( 100000 );

	// ちょっと加速
	Spur_set_vel( 0.3 );
	Spur_set_accel( 1.0 );
	Spur_set_angvel( M_PI  );
	Spur_set_angaccel( M_PI );
	
	// 走行
	Spur_stop_line_GL( 1.0, 0.1, M_PI / 2.0 );
	while( !Spur_over_line_GL( 1.0, 0.1 - 0.005, M_PI / 2.0 ) )
		usleep( 100000 );
	
	// 回転
	printf( "spin\n" );
	Spur_spin_GL( M_PI  );
	while( !Spur_near_ang_GL( M_PI, M_PI / 18.0 ) )
		usleep( 100000 );

	// 走行
	printf( "line\n" );
	Spur_stop_line_GL( 0, 0.1, M_PI );
	while( !Spur_over_line_GL( 0.0 + 0.005, 0.1, M_PI ) )
		usleep( 100000 );

	// 回転
	printf( "spin\n" );
	Spur_spin_GL( -M_PI / 2.0 );
	while( !Spur_near_ang_GL( -M_PI/2.0, M_PI / 18.0 ) )
		usleep( 100000 );

	// 走行
	printf( "line\n" );
	Spur_stop_line_GL( 0, 0.0, -M_PI / 2.0 );
	while( !Spur_over_line_GL( 0.0, 0.0 + 0.005, -M_PI / 2.0 ) )
		usleep( 100000 );

	// 回転
	printf( "spin\n" );
	Spur_spin_GL( 0 );
	while( !Spur_near_ang_GL( 0, M_PI / 18.0 ) )
		usleep( 100000 );

	Spur_stop(  );
	usleep( 4000000 );
	Spur_free(  );
	printf( "Hit Ctrl-C to exit.\n" );
	while( 1 )
	{
		Spur_get_pos_GL( &x, &y, &theta );
		printf( "%f %f %f\n", x, y, theta * 180.0 / M_PI );
		usleep( 1000000 );
	}
	
	return 0;
}
