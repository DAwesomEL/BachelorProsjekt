#include "magI2C.h"
#include "Wire.h"
#include "arduino.h"



MagI2CUnit::MagI2CUnit(int ID, int &deviceCounter)
{
	deviceCounter++;
	slaveAdress = ID; // I2C address of sensor
	magnetStatus = 0;
}

void MagI2CUnit::I2CComm()
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
int MagI2CUnit::I2CScan()
	{
	byte err, adr;       /*variable error is defined with address of I2C*/
	adr = slaveAdress;
	Wire.beginTransmission(adr);
	err = Wire.endTransmission();

	if (err == 0)
	{
		Serial.print("The I2C device at address 0x");
		if (adr < 16)
			Serial.print("0");
		Serial.print(adr, HEX);
		Serial.println(" is indeed there.");
		return 0;
	}
	else if (err == 4)
	{
		Serial.print("Unknown error at address 0x ");
		if (adr < 16)
			Serial.print("0");
		Serial.print(adr, HEX);
		Serial.println(" but the program can see something at this address");
		return 0;
	}
	else { 
		Serial.println("I2CScan failed, most likely due to the sensor not being connected or having wrong adress");
		return 1;
	}
}

int MagI2CUnit::getID(bool printMe)
{
	if (printMe) {
		Serial.print("Slave Adress: ");
		Serial.println(slaveAdress, HEX);
	}
	return slaveAdress;
}


float MagI2CUnit::readAngle()
{
	int lowbyte; //raw angle 7:0
	word highbyte; //raw angle 7:0 and 11:8
	int rawAngle; //final raw angle 
	

	//7:0 - bits
	Wire.beginTransmission(0x36); //connect to the sensor
	Wire.write(0x0D); //figure 21 - register map: Raw angle (7:0)
	Wire.endTransmission(); //end transmission
	Wire.requestFrom(0x36, 1); //request from the sensor

	while (Wire.available() == 0); //wait until it becomes available 
	lowbyte = Wire.read(); //Reading the data after the request

	//11:8 - 4 bits
	Wire.beginTransmission(0x36);
	Wire.write(0x0C); //figure 21 - register map: Raw angle (11:8)
	Wire.endTransmission();
	Wire.requestFrom(0x36, 1);

	while (Wire.available() == 0);
	highbyte = Wire.read();


	highbyte = highbyte << 8; //shifting to left

	rawAngle = highbyte | lowbyte; //int is 16 bits (as well as the word)

	trueAngle = rawAngle * ((float)360 / 4096); //angle in degrees (360/4096 * [value between 0-4095])

	// Serial.print("Deg angle: ");
	// Serial.println(degAngle, 2); //absolute position of the encoder within the 0-360 circle

	return trueAngle;
}


void MagI2CUnit::checkMagnetPresence()
{
	int lastMagnetStatus = 0;
	//This function runs in the setup() and it locks the MCU until the magnet is not positioned properly

	while ((magnetStatus & 32) != 32) //while the magnet is not adjusted to the proper distance - 32: MD = 1
	{
		magnetStatus = 0; //reset reading

		Wire.beginTransmission(0x36); //connect to the sensor
		Wire.write(0x0B); //figure 21 - register map: Status: MD ML MH
		Wire.endTransmission(); //end transmission
		Wire.requestFrom(0x36, 1); //request from the sensor

		while (Wire.available() == 0); //wait until it becomes available 
		magnetStatus = Wire.read(); //Reading the data after the request

		if (lastMagnetStatus != magnetStatus) {
			Serial.print("Magnet status: ");
			Serial.println(magnetStatus, BIN); //print it in binary so you can compare it to the table (fig 21)   
		} 
		lastMagnetStatus = magnetStatus;
	}

	//Status register output: 0 0 MD ML MH 0 0 0  
	//MH: Too strong magnet - 100111 - DEC: 39 
	//ML: Too weak magnet - 10111 - DEC: 23     
	//MD: OK magnet - 110111 - DEC: 55

	Serial.println("Magnet found!");
	Serial.println("");
	delay(1000);
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
