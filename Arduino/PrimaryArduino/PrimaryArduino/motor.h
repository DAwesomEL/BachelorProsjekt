#pragma once
class Motor
{ 
public:
	Motor(int cwNew, int ccwNew, float KpNew, float KiNew, float KdNew);
	~Motor() = default;

	int assignedPins(int cwNew, int ccwNew);
	int PIDvalues(float KpNew, float KiNew, float KdNew);

	int PID(double ang, double desAng, long int time);

	int drive();

	int cw;
	int ccw;
	float Kp;
	float Ki;
	float Kd;
	int PWM;
	bool dir;
	float summedError;
	long int lastTime;
	float lastAng;
};
