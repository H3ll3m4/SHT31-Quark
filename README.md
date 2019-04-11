# SHT31-Quark
Drivers for Sensirion SHT31 for tinyTILE - Quark

Example .ino file:
#include "RHTsensorSHT31.h"
RHTsensorSHT31 sensor = RHTsensorSHT31();

void setup() {
  Serial.begin(115200);
}

void loop() {
  sensor.updateRHT();
}
