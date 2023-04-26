/*
 Name:		PrimaryArduino.ino
 Created:	4/25/2023 5:10:09 PM
 Author:	dngmo
*/

// the setup function runs once when you press reset or power the board
#include "Wire.h"
#include "arduino.h"
#include "motor.h"
#include "serial.h"
#include "I2C.h"
// #include "sensor.h"

#define SLAVE_ADDRESS_1 36 // I2C address of sensor #1
#define SLAVE_ADDRESS_2 9 // I2C address of sensor #2


Motor* motorRS = NULL;
Motor* motorRT = NULL;
Motor* motorRE = NULL;


int numberOfI2CDevices = 0;

I2CUnit magRElbow(0x36, numberOfI2CDevices);

long int time;
long int startTime;
bool firstRun = true;


// Test variables, delete if you don't remember what it was for, and all code that gets errors because of it
int count = 0;

// the setup function runs once when you press reset or power the board
void setup() {
	time = 0;
	startTime = millis();
	Serial.begin(19200);
	Serial1.begin(9600);
	Serial.flush();
	Serial1.flush();
	Wire.begin();
}

void motorSetup() {
	motorRS->assignedPins(5, 6);
	motorRS->PIDvalues(5, 0.1, 3);

	motorRT->assignedPins(7, 8);
	motorRT->PIDvalues(2, 1, 0);

	motorRE->assignedPins(9, 10);
	motorRT->PIDvalues(1, 0, 0);
}

void I2CSetup() 
{
	magRElbow.getID(true);
	magRElbow.I2CScan();
	magRElbow.I2CComm();
	/*I2CUnit magRSholder(37, numberOfI2CDevices);
	I2CUnit imuRFemurInner(38, numberOfI2CDevices);
	I2CUnit imuRFemurOuter(39, numberOfI2CDevices);
	I2CUnit SVEIIIIIIIIIIIN(40, numberOfI2CDevices);
	*/
	// automatic check if there are the same amount of sensors that are connected and that are defined.
	int i2cDevices = I2CScanGeneral();
	if (i2cDevices < numberOfI2CDevices){
		Serial.print("Warning! You have too many defined sensors. There are ");
		Serial.print(i2cDevices);
		Serial.print(" sensors, and you have defined ");
		Serial.println(numberOfI2CDevices);
	}
	else if(i2cDevices > numberOfI2CDevices){
		Serial.print("Warning! You have too few defined sensors. There are ");
		Serial.print(i2cDevices);
		Serial.print(" sensors, and you have defined ");
		Serial.println(numberOfI2CDevices);
	}
	else {
		Serial.print("There are a total of: ");
		Serial.print(numberOfI2CDevices);
		Serial.println(" devices.");
	}
}

// the loop function runs over and over again until power down or reset
void loop() 
{
	if (firstRun) {
		firstRun = false;
		motorSetup();
	}
	I2CSetup();
	time = millis();

	if (time %1000 == 0) { // 134 av 600 på 6007 ms //60 av 60 på 5989 ms // 118 av 120 på 5989 ms // 1890 av 2000 på 95989 ms // 960 av 960 på 95989
		// uten noen annen stress på systemet. Ikke send serial mer enn 10 ganger i sekundet.
		//Serial.println("Du heter svein?");
	}
}