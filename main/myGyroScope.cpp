#include "myGyroScope.hpp"

float RateRoll, RatePitch, RateYaw;
int16_t GyroX, GyroY, GyroZ;

void gyro_signals(void) {

  // Choose Low-Pass filter with a cutoff of 10Hz.
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);
  Wire.write(0x05);
  Wire.endTransmission();

  // Choose Sensititivity of the gyroscope 65.5 LSB/deg/s.
  Wire.beginTransmission(0x68);
  Wire.write(0x1B); 
  Wire.write(0x8); 
  Wire.endTransmission(); 

  //  The Gyro Output is a 16 bit value transmitted as two 8 bit values
  //  Concatonate two 8 bit values to get the 16 bit value.
  Wire.beginTransmission(0x68);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(0x68,6);
  GyroX=Wire.read()<<8 | Wire.read();
  GyroY=Wire.read()<<8 | Wire.read();
  GyroZ=Wire.read()<<8 | Wire.read();

  // GyroOut is in the form GyroOut = Sensitivity*AngularRate. 
  RateRoll=(float)GyroX/65.5;
  RatePitch=(float)GyroY/65.5;
  RateYaw=(float)GyroZ/65.5;
}
