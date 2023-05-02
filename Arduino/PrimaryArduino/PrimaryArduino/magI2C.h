#pragma once

class MagI2CUnit {
public: 
	MagI2CUnit(int ID, int &deviceCounter);
	void I2CComm();
	int getID(bool printMe);
	int I2CScan();

	float readAngle();

	void checkMagnetPresence();
	float trueAngle;
	int magnetStatus;
	int slaveAdress;
};

int I2CScanGeneral();
