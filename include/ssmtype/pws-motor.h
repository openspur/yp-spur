#ifndef __PWS_MOTOR_H__
#define __PWS_MOTOR_H__

#define SNAME_PWS_MOTOR "pws_motor"

typedef struct _pws_motor *PWSMotorPtr;
typedef struct _pws_motor
{
	int counter1;
	int counter2;
	int pwm1;
	int pwm2;
} PWSMotor;

#endif
