#include "main_arduino.hpp"

const unsigned long BaudRate{57600};
const uint32_t ArdClock{400000};


void main_setup(void) {
  Serial.begin(BaudRate);
  //pinMode(13, OUTPUT);
  //digitalWrite(13, HIGH);
  Wire.setClock(ArdClock);
  Wire.begin();
  delay(250);

  // Select Clock Source
  Wire.beginTransmission(I2C_ADD);
  Wire.write(PWR_MGMT_1);
  Wire.write(CLKSEL_20MHZ);
  Wire.endTransmission();
}

