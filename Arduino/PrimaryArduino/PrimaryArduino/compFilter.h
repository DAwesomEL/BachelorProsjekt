#pragma once
#include "mpu9250.h"
class compFilter
{
public:
	compFilter() = delete;
	compFilter(float alpaNew, float betaNew);
	void getAngles(bfs::Mpu9250 imu, float dt);
	void printAngles();
	float alpha;
	float beta;
	float roll;
	float pitch;
	float yaw;
	float accelX_filt;
	float accelY_filt;
	float accelZ_filt;
	float gyroX_filt;
	float gyroY_filt;
	float gyroZ_filt;
};

