#include "I2C.h"
#include "Wire.h"
#include "arduino.h"



I2CUnit::I2CUnit(int ID, int &deviceCounter)
{
	deviceCounter++;
	slaveAdress = ID; // I2C address of sensor
}

void I2CUnit::I2CComm()
{
	// Read data from sensor #1
	Wire.beginTransmission(slaveAdress);
	Wire.write(0x00); // Send a request to read data from the sensor
	Wire.endTransmission();
	Wire.requestFrom(slaveAdress, 2); // Read 2 bytes of data from the sensor
	int data = Wire.read() << 8 | Wire.read(); // Combine the two bytes into a single integer value


	Serial.print("The test sensor's value is: ");
	Serial.println(data);
}
void I2CUnit::I2CScan()
	{
	byte err, adr;       /*variable error is defined with address of I2C*/
	adr = slaveAdress;
	Wire.beginTransmission(adr);
	err = Wire.endTransmission();

	if (err == 0)
	{
		Serial.print("I2C device at address 0x");
		if (adr < 16)
			Serial.print("0");
		Serial.print(adr, HEX);
		Serial.println(" was found.");
	}
	else if (err == 4)
	{
		Serial.print("Unknown error at address 0x");
		if (adr < 16)
			Serial.print("0");
		Serial.println(adr, HEX);
	}
}

int I2CUnit::getID(bool printMe)
{
	if (printMe) {
		Serial.print("Slave Adress: ");
		Serial.println(slaveAdress);
	}
	return slaveAdress;
}


int I2CScanGeneral()
{
	byte err, adr;       /*variable error is defined with address of I2C*/
	int numberOfDevices = 0;
	Serial.println("Scanning.");
	for (adr = 1; adr < 127; adr++)
	{
		Wire.beginTransmission(adr);
		err = Wire.endTransmission();

		if (err == 0)
		{
			Serial.print("I2C device at address 0x");
			if (adr < 16)
				Serial.print("0");
			Serial.print(adr, HEX);
			Serial.println("  !");
			numberOfDevices++;
		}
		else if (err == 4)
		{
			Serial.print("Unknown error at address 0x");
			if (adr < 16)
				Serial.print("0");
			Serial.println(adr, HEX);
		}
	}
	if (numberOfDevices == 0)
		Serial.println("No I2C devices attached\n");
	else
		Serial.println("done\n");
	return numberOfDevices;
}
