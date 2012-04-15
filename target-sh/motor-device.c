/* ------------------------------------------------------- */
/* SH2/7045でモータを操作するための関数群 　　　 */
/* ISSのプログラムベースで作成 */
/* ------------------------------------------------------- */
#include<sh7040s.h>
#include<motor-device.h>
#include<sh-globals.h>

/* いじるときはここではなく下をいじる */
#define SH_MOTOR_LEFT   	0					/* モータの左 */
#define SH_MOTOR_RIGHT		1					/* モータの右 */

/* 
 * B-Locoボードのコネクタでの配置
 * 右と左を入れ替えるときはこちらを変更 
 */
#define MOTOR_ID_CON0		(SH_MOTOR_RIGHT)
#define MOTOR_ID_CON1		(SH_MOTOR_LEFT)

/* モータの回転方向を 正:0, 逆:1 */
#define MOTOR_IS_REVERSE		1

/*-----------------PWMの関数-------------------*/
volatile int duty_buffer[2];
volatile unsigned char duty_set = 0; 
volatile unsigned short pe;

// //////////////////////// init PWM //////////////////////////////////
void initPWM(  )
{
	int i;

	PFC.PEIOR.WORD |= 0x0500;					/* use TIOC3A, TIOC3C port */
	MTU3.TCR.BYTE = 0x20;						/* cycle=TGRA, duty=TGRB */
	MTU3.TMDR.BYTE = 0xc2;						/* pwm mode 1, buffer disable */
	MTU3.TIOR.BYTE.H = 0x12;					/* CMA=0, CMB=1 */
	MTU3.TIOR.BYTE.L = 0x12;					/* CMC=0, CMD=1 */
	MTU3.TGRA = MTU3.TGRC = MTR_PWM_DEFAULT_MAX;	/* cycle */
	pwm_max[0] = MTR_PWM_DEFAULT_MAX;
	pwm_max[1] = MTR_PWM_DEFAULT_MAX;
	pwm_min[0] = -MTR_PWM_DEFAULT_MAX;
	pwm_min[1] = -MTR_PWM_DEFAULT_MAX;

	PFC.PECR1.WORD |= 0x0011;					/* use TGRA, TGRC port */

	MTU.TSTR.BYTE |= 0xc0;						/* pwm active */

	PFC.PECR1.WORD &= ~0x0044;					/* TIOC3B, TIOC3D */
	PFC.PECR2.WORD &= ~0x4400;					/* TIOC1B, TIOC2B */
	PFC.PEIOR.WORD |= 0x0aa0;

	pe = PE.DR.WORD;
	for ( i = 0; i < 2; i++ )
	{
		put_pwm( i, 0 );
	}
	MTU3.TGRB = 0;
	MTU3.TGRD = 0;
	duty_set = 0;
}

// ///////////////// デューティー比設定 pwm_set //////////////////////

void set_pwm( const unsigned char id, const int duty )
{
	if( id == MOTOR_ID_CON0 )
	{
		duty_buffer[0] = duty;
		duty_set |= 0x1;
	}
	else if( id == MOTOR_ID_CON1 )
	{
		duty_buffer[1] = duty;
		duty_set |= 0x2;
	}
}

void set_mode( const unsigned char id, const int mode )
{
	if( id == MOTOR_ID_CON0 )
	{
		/* DIR(PE5), EN(PE7) */
		if( mode == MTR_MODE_CCW_BRAKE )
		{
			pe &= ~0x0020;				/* DIR(PE5)=1 */
			pe &= ~0x0080;				/* EN(PE7)=0 */
		}
		else if( mode == MTR_MODE_CW_BRAKE )
		{
			pe |=  0x0020;				/* DIR(PE5)=1 */
			pe &= ~0x0080;				/* EN(PE7)=0 */
		}
		else
		{										/* MTR_MODE_FREE */
			pe |=  0x0080;				/* EN(PE7)=1 */
		}

	}
	else if( id == MOTOR_ID_CON1 )
	{
		/* DIR(PE9), EN(PE11) */
		if( mode == MTR_MODE_CCW_BRAKE )
		{
			pe &= ~0x0200;				/* DIR(PE9)=1 */
			pe &= ~0x0800;				/* EN(PE11)=1 */
		}
		else if( mode == MTR_MODE_CW_BRAKE )
		{
			pe |=  0x0200;				/* DIR(PE9)=0 */
			pe &= ~0x0800;				/* EN(PE11)=1 */
		}
		else
		{										/* MTR_MODE_FREE */
			pe |=  0x0800;				/* EN(PE11)=1 */
		}
	}
}

void put_pwm( int ch, int pwm )
{
#if MOTOR_IS_REVERSE
	pwm = -pwm;
#endif
	if( duty_set == 0 )
	{
		pe = PE.DR.WORD;
	}
	if( pwm < 0 )
	{
		set_mode( ch, MTR_MODE_CW_BRAKE );
		set_pwm( ch, -pwm );
	}
	else
	{
		set_mode( ch, MTR_MODE_CCW_BRAKE );
		set_pwm( ch, pwm );
	}
	if( duty_set == 0x3 )
	{
		register unsigned short tgrb;
		register unsigned short tgrd;
		tgrb = duty_buffer[0];
		tgrd = duty_buffer[1];
		for( ; MTU3.TCNT < pwm_max[0] - 16;  );
		PE.DR.WORD = pe;
		MTU3.TGRB = tgrb;
		MTU3.TGRD = tgrd;
		MTU3.TCNT = 0;

		duty_set = 0;
	}
}

/*--------------------------カウンタの関数-----------------------------------*/

// /////////////////////// init counter ///////////////////////////

void initCounter( void )
{
	MTU1.TMDR.BYTE = 0xc4;						/* encoder mode 1 */
	MTU2.TMDR.BYTE = 0xc4;						/* encoder mode 1 */
	MTU.TSTR.BYTE |= 0x06;						/* start count up */

	PFC.PECR2.WORD |= 0x1100;					/* TIOC1A, TIOC2A */
	MTU1.TIOR.BYTE  = 0x0a;						/* input capture */
	MTU2.TIOR.BYTE  = 0x0a;						/* input capture */

	PFC.PEIOR.WORD &= ~0x0050;					/* use TIOC1A, TIOC2A port */

	PFC.PACRL2.WORD &= ~0xf000;					/* TCLKA, TCLKB */
	PFC.PACRL2.WORD |=  0x5000;
	PFC.PACRL1.WORD &= ~0x000f;					/* TCLKC, TCLKD */
	PFC.PACRL1.WORD |=  0x0005;

	MTU1.TCNT = 0;								/* counter=0 */
	MTU2.TCNT = 0;
	MTU1.TGRA = 0;								/* buffer=0 */
	MTU2.TGRA = 0;
}

// ///////////////// カウント値取り込み cnt_read ////////////////////
void cnt_read( void )
{

	static short cnt_old[2];

	short int i;

#if MOTOR_IS_REVERSE
	counter[MOTOR_ID_CON0] = -( short )( ( volatile )MTU1.TCNT );
	counter[MOTOR_ID_CON1] = -( short )( ( volatile )MTU2.TCNT );
#else
	counter[MOTOR_ID_CON0] = ( volatile )MTU1.TCNT;
	counter[MOTOR_ID_CON1] = ( volatile )MTU2.TCNT;
#endif

	for ( i = 0; i < 2; i++ )
	{
		cnt_dif[i] = ( short )( counter[i] - cnt_old[i] );
		cnt_old[i] = counter[i];
	}
}

