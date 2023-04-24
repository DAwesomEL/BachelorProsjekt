#pragma once
class Sensor
{
public:
	~Sensor() = default;

	virtual int i2c() = 0;
};

