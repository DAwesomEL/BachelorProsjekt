#include "compFilter.h"
#include "mpu9250.h"

compFilter::compFilter(float alphaNew, float betaNew)
{
	alpha = alphaNew;
	beta = betaNew;
	roll = 0;
	pitch = 0;
	yaw = 0;
	accelX_filt = 0.0;
	accelY_filt = 0.0;
	accelZ_filt = 0.0;
	gyroX_filt = 0.0;
	gyroY_filt = 0.0;
	gyroZ_filt = 0.0;
}

void compFilter::getAngles(bfs::Mpu9250 imu, float dt)
{

	if (imu.Read()) {
		accelX_filt = alpha * accelX_filt + beta * imu.accel_x_mps2();
		accelY_filt = alpha * accelY_filt + beta * imu.accel_y_mps2();
		accelZ_filt = alpha * accelZ_filt + beta * imu.accel_z_mps2();

		// Apply filter to gyroscope data
		gyroX_filt = alpha * gyroX_filt + beta * imu.gyro_x_radps();
		gyroY_filt = alpha * gyroY_filt + beta * imu.gyro_y_radps();
		gyroZ_filt = alpha * gyroZ_filt + beta * imu.gyro_z_radps();

		// Calculate roll and pitch angles
		roll = atan2(accelY_filt, accelZ_filt) * RAD_TO_DEG;
		pitch = atan(-accelX_filt / sqrt(pow(accelY_filt, 2) + pow(accelZ_filt, 2))) * RAD_TO_DEG;

		// Calculate yaw angle from gyroscope data
		yaw = gyroZ_filt * dt
	}
}

void compFilter::printAngles()
{
	// Print angles to serial monitor
	Serial.print("Roll: ");
	Serial.print(roll, 2);
	Serial.print(", Pitch: ");
	Serial.print(pitch, 2);
	Serial.print(", Yaw: ");
	Serial.println(yaw, 2);
}
