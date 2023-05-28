#include "serial.h"
#include "motor.h"
#include "arduino.h"
int serialComm()
{
	int val = 1000;
	String data;
	// Send the data to the secondary Arduino over TX/RX
	Serial.print("Todays number: ");
	Serial.println("69");

	// Read data from the secondary Arduino over TX/RX
	if (Serial.available() > 0) {
		data = Serial.readStringUntil('\n');
	}
	if (val < -360 || val > 360) {
		int val = data.toInt();
	}
	return val;
}