#ifndef _RHTSENSORSHT31_h
#define _RHTSENSORSHT31_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <quarkI2C.h>

/*RH+T Characteristic */
#define ADDRESS_SHT31 0x44
/*MSB and LSB defined for medium repeatability and no clock-stretching for single shot mode */
#define CMD_MSB 0x24
#define CMD_LSB 00
/*SHT31 Commands */
#define S_RST 0x30A2
#define STAT_REG 0xF32D // read out status register
#define CLEAR 0x3041 //Clear Status register

class RHTsensorSHT31{
public:
	RHTsensorSHT31();
	bool updateRHT();
	int measurementDuration;
	double getHumidity();
	double getTemp();
	void reset();

private:
	double _humidity;
	double _temp;
	int CRC8(int * data, int len);
};

#endif

