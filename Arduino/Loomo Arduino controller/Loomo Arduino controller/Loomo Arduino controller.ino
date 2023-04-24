/*
 Name:		Loomo_Arduino_controller.ino
 Created:	4/24/2023 11:54:30 AM
 Author:	dngmo
*/
#include "Motor.h"
//#include "arduino.h"


Motor* motorRS = NULL;
Motor* motorRT = NULL;
Motor* motorRE = NULL;

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
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
	Serial.print("SVEIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIN!\n");
	delay(1000);
}
