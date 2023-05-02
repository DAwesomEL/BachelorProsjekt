#include "serial.h"
#include "motor.h"
#include "arduino.h"
void serialComm(Motor &motor)
{
	// Send the data to the secondary Arduino over TX/RX
	Serial1.print("Dagens tall: ");
	Serial1.println("Du er en noob");

	// Read data from the secondary Arduino over TX/RX
	if (Serial1.available() > 0) {
		String data = Serial1.readStringUntil('\n');
		Serial.println("Received data from secondary Arduino: " + data);
	}
	else { Serial.println("NOTHING REALLY?"); }
	Serial.print("BTW, motorRS' Kp value is: ");
	Serial.println(motor.Kp);
}