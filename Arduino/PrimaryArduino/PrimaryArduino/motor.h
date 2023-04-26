#pragma once
class Motor
{ 
public:
	Motor();
	~Motor() = default;

	int assignedPins(int cw, int ccw);
	int PIDvalues(int Kp, int Ki, int Kd);

	int PID(double ang, double desAng);

	int drive();

	int Kp;
private:
	int cw;
	int ccw;
	int Ki;
	int Kd;
	int PWM;
	bool dir;
	double summedError;
	long int lastTime;
	double lastAng;
};
