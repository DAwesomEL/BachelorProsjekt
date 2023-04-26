#include "motor.h"
#include "arduino.h"
Motor::Motor()
{
	summedError = 0;
	lastTime = 0;
	lastAng = 0;
}


int Motor::assignedPins(int cw, int ccw)
{
	this->cw = cw;
	this->ccw = ccw;
	pinMode(cw, INPUT);
	pinMode(ccw, INPUT);
	return 0;
}

int Motor::PIDvalues(int Kp, int Ki, int Kd)
{
	this->Kp = Kp;
	this->Ki = Ki;
	this->Kd = Kd;
	return 0;
}


int Motor::PID(double ang, double desAng)
{
	double P;
	double I;
	double D;
	long int time = millis();

	// Proportional
	P = Kp * (desAng - ang);

	// Integrator
	summedError += (desAng - ang);
	I = abs(summedError);
	if (summedError > 0) {
		dir = 1;
	}
	else {
		dir = 0;
	}

	// Derivator
	D = Kd * (ang - lastAng) / (time - lastTime);

	//PWM
	PWM = P + I + D;

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
		analogWrite(ccw, PWM);
		analogWrite(cw, 0);
	}

	return 0;
}

