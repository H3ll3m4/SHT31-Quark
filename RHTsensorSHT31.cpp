#include "RHTsensorSHT31.h"

RHTsensorSHT31::RHTsensorSHT31() {
	_temp = 0;
	_humidity = 0;
	measurementDuration = 15;// 15max, 12.5 ms typical according to Sensirion documentation. 
	I2C0.begin();
}



bool RHTsensorSHT31::updateRHT() {
	/* Connect to SHT31 Sensor via I2C to get RH and T
	*  Display values via Serial and BLE
	*/
	int temperatureArray [3];
	int RHArray [3];
	double oldTemp = _temp;
	double oldHumidity = _humidity;
	double tempMeasure = 0;
	double humidityMeasure = 0;

	//Serial.println("Measurement starting");
	//I2C Initialization 
	I2C0.begin();        // join i2c bus
	//16 bits measurement command   
	I2C0.write((byte)CMD_MSB) ;
	I2C0.write((byte)CMD_LSB);
	I2C0.endTransmission();
	delay(measurementDuration);//Measurement on-going
	//Measurements
	int ret = I2C0.requestFrom(ADDRESS_SHT31, 6);    // request 6 bytes from slave device #68
	  
	if (ret == 0)
	{
		Serial.println("read from slave device failed");   
	}
		
	if (6 <= I2C0.available()) {
	   for (int i = 0 ; i<3 ; i++)
		  temperatureArray[i] = I2C0.read();
	   for (int i = 0 ; i<3 ; i++)
		  RHArray[i] = I2C0.read();
	  }
	I2C0.endTransmission();
	  
	tempMeasure = temperatureArray[1] + (temperatureArray[0] << 8);
	if (temperatureArray[2] !=  CRC8(temperatureArray, 2)){
		Serial.println("CRC false");
		return false;
	  }
	//16 bits value to be converted into a physical scale
	 _temp = (175 * tempMeasure / (pow(2, 16) - 1)) - 45;
	 if (_temp != oldTemp) {
		oldTemp = _temp;
	  }

	humidityMeasure = RHArray[1] + (RHArray[0] << 8);
	if (RHArray[2] !=  CRC8(RHArray, 2)){
		Serial.println("CRC false");
		return false;
	  }
	//16 bits value to be converted into a physical scale
	_humidity = 100 * humidityMeasure / (pow(2, 16) - 1);
	if (_humidity != oldHumidity) {
		oldHumidity = _humidity;
	  }
	 return true;
}

double RHTsensorSHT31::getHumidity(){
	Serial.print("Sensor : Humidity = ");
	Serial.println(_humidity, 10);
	return (_humidity);
}

double RHTsensorSHT31::getTemp() {
	Serial.print("Sensor : Temperature = ");
	Serial.println(_temp, 10);
	return (_temp);
}

void RHTsensorSHT31::reset(){
	I2C0.begin();        // join i2c bus
	//16 bits measurement command   
	I2C0.write((byte)S_RST) ;
	I2C0.endTransmission();
}

//CRC8 using polynomial
//0x31 (x 8 + x 5 + x 4 + 1)
int RHTsensorSHT31::CRC8(int * data, int len){
	const uint8_t POLYNOMIAL(0x31);
	uint8_t crc(0xFF);

	for (int j = len; j; --j) {
		crc ^= *data++;

		for (int i = 8; i; --i) {
			crc = (crc & 0x80) ? (crc << 1) ^ POLYNOMIAL : (crc << 1);
		}
	}
	return crc;
  }
