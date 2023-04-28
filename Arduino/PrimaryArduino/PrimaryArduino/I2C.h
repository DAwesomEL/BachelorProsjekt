#pragma once

class I2CUnit {
public: 
	I2CUnit(int ID, int &deviceCounter);
	void I2CComm();
	int getID(bool printMe);
	void I2CScan();

	float readAngle();

	void checkMagnetPresence();
	float trueAngle;
	int magnetStatus;
	int slaveAdress;
};

int I2CScanGeneral();
