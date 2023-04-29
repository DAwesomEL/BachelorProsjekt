/*
 Name:		PrimaryArduino.ino
 Created:	4/25/2023 5:10:09 PM
 Author:	dngmo
*/

// the setup function runs once when you press reset or power the board
#include <invensense_imu.h>
#include "Wire.h"
#include "arduino.h"
#include "motor.h"
#include "serial.h"
#include "magI2C.h"
#include "mpu9250.h"
#include "mpu6500.h"



int numberOfI2CDevices = 0;


Motor motorRS(2, 3, 2, 0, 0);
Motor motorRT(4, 5, 2, 0, 0);
Motor motorRE(6, 7, 2, 0, 0);

bfs::Mpu9250 mpuRUpper(&Wire, bfs::Mpu9250::I2C_ADDR_PRIM);
bfs::Mpu9250 mpuRLower(&Wire, bfs::Mpu9250::I2C_ADDR_SEC);
MagI2CUnit magRE(0x36, numberOfI2CDevices);
MagI2CUnit magRS(0x71, numberOfI2CDevices);

long int time;
long int startTime;
bool firstRun = true;


// Test variables, delete if you don't remember what it was for, and all code that gets errors because of it


// the setup function runs once when you press reset or power the board
void setup() {
	time = 0;
	startTime = millis();

	Serial.begin(115200);
	Serial1.begin(9600);	
	Serial.flush();
	Serial1.flush();
	Wire.begin();
	Wire.setClock(400000L); //fast clock

	mpuRUpper.Begin();
	numberOfI2CDevices++;
	mpuRLower.Begin();
	numberOfI2CDevices++;
	
	/*for (int i = 0; 11; i++) {
		pinMode(i+1, OUTPUT);
	}*/
	

	Serial.println("Starting..");
	firstRun = false;
	I2CSetup();
}



// the loop function runs over and over again until power down or reset
void loop() 
{
	time = millis();
	if (time % 100 == 0) {
		Serial.write("");
		magRE.readAngle();
		motorRS.PID(magRE.trueAngle, 200, time);
		motorRS.drive();
	}

	if (time % 1000 == 0) { // 134 av 600 p� 6007 ms //60 av 60 p� 5989 ms // 118 av 120 p� 5989 ms // 1890 av 2000 p� 95989 ms // 960 av 960 p� 95989
		// uten noen annen stress p� systemet. Ikke send serial mer enn 10 ganger i sekundet.
		Serial.print("PWM: ");
		Serial.println(motorRS.PWM);
		Serial.print("Kp value: ");
		Serial.println(motorRS.Kp);
		Serial.print("True Angle: ");
		Serial.println(magRE.trueAngle);
	}
}


// -------------------------------------------------------------------------------------

void I2CSetup() 
{
	delay(100);
	magRE.getID(true);
	magRE.I2CScan();
	magRE.checkMagnetPresence();
	// automatic check if there are the same amount of sensors that are connected and that are defined.
	int i2cDevices = I2CScanGeneral();
	if (i2cDevices != numberOfI2CDevices){

		if (i2cDevices < numberOfI2CDevices) { Serial.print("Warning! You have too many defined sensors. There are "); }
		else if (i2cDevices > numberOfI2CDevices) { Serial.print("Warning! You have too few defined sensors. There are "); }

		Serial.print(i2cDevices);

		if(numberOfI2CDevices == 1){ Serial.print(" sensor, "); }
		else { Serial.print(" sensors, "); }
		
		Serial.print("and you have defined ");
	}
	else {
		Serial.print("There are a total of: ");
	}
	Serial.print(numberOfI2CDevices);
	Serial.println(" devices.");
}