#include "motor.h"
#include "arduino.h"
Motor::Motor(int cwNew, int ccwNew, float KpNew, float KiNew, float KdNew)
{
	cw = cwNew;
	ccw = ccwNew;
	Kp = KpNew;
	Ki = KiNew;
	Kd = KdNew;

	summedError = 0;
	lastTime = 0;
	lastAng = 0;
	dir = true;
}


int Motor::assignedPins(int cwNew, int ccwNew)
{
	cw = cwNew;
	ccw = ccwNew;
	pinMode(cw, OUTPUT);
	pinMode(ccw, OUTPUT);
	return 0;
}

int Motor::PIDvalues(float KpNew, float KiNew, float KdNew)
{
	Kp = KpNew;
	Ki = KiNew;
	Kd = KdNew;
	return 0;
}


int Motor::PID(double ang, double desAng, long int time)
{
	float P = 0;
	float I = 0;
	float D = 0;

	float error = desAng - ang;


	// At 360 degrees the angle swaps around, if the current angle is at 358 degrees and the desired angle is at 1, 
	// they are in reality just 3 degrees apart, but the error would be 357
	if (error > 180)
		error = error - 360;
	else if (error < -180)
		error = error + 360;

	float absError = abs(error);
	// Proportional
	P = Kp * absError;
	//PWM
	PWM = (int)round(abs(P + I + D));
	;
	if (PWM >= 255)
		PWM = 255;


	if (error < 0) {
		dir = false;
	}
	else {
		dir = true;
	}

	//remember last values
	lastAng = ang;
	lastTime = time;
	return 0;
}

int Motor::drive()
{
	if (dir) {
		analogWrite(cw, PWM);
		analogWrite(ccw, 0);
	}
	else {
		analogWrite(cw, 0);
		analogWrite(ccw, PWM);
	}

	return 0;
}

