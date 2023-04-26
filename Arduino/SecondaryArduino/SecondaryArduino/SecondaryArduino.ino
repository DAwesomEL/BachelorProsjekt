/*
 Name:		SecondaryArduino.ino
 Created:	4/25/2023 5:27:18 PM
 Author:	dngmo
*/

#include <Wire.h>
#include "arduino.h"
#include "motor.h"

#define SLAVE_ADDRESS_1 8 // I2C address of sensor #1
#define SLAVE_ADDRESS_2 9 // I2C address of sensor #2


Motor* motorRS = NULL;
Motor* motorRT = NULL;
Motor* motorRE = NULL;

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(19200);
	Serial1.begin(9600);
	Wire.begin();
	motorSetup();
}

void motorSetup() {
	motorRS->assignedPins(5, 6);
	motorRS->PIDvalues(5, 0.1, 3);

	motorRT->assignedPins(7, 8);
	motorRT->PIDvalues(2, 1, 0);

	motorRE->assignedPins(9, 10);
	motorRT->PIDvalues(1, 0, 0);
}

// the loop function runs over and over again until power down or reset
void loop() {
	// Read data from sensor #1
	Wire.beginTransmission(SLAVE_ADDRESS_1);
	Wire.write(0); // Send a request to read data from the sensor
	Wire.endTransmission();
	Wire.requestFrom(SLAVE_ADDRESS_1, 2); // Read 2 bytes of data from the sensor
	int data1 = Wire.read() << 8 | Wire.read(); // Combine the two bytes into a single integer value

	// Read data from sensor #2
	Wire.beginTransmission(SLAVE_ADDRESS_2);
	Wire.write(0); // Send a request to read data from the sensor
	Wire.endTransmission();
	Wire.requestFrom(SLAVE_ADDRESS_2, 2); // Read 2 bytes of data from the sensor
	int data2 = Wire.read() << 8 | Wire.read(); // Combine the two bytes into a single integer value


	// Read data from the primary Arduino over TX/RX
	if (Serial1.available() > 0) {
		String data = Serial1.readStringUntil('\n');
		Serial.println("Rodger that primary, received message: " + data);
		Serial1.println("Rodger that primary, received message: " + data);
	}

	delay(5000); // Wait for a second before repeating the loop
}