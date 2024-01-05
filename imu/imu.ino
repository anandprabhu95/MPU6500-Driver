#include <Wire.h>


float RateRoll, RatePitch, RateYaw;


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
  int16_t GyroX=Wire.read()<<8 | Wire.read();
  int16_t GyroY=Wire.read()<<8 | Wire.read();
  int16_t GyroZ=Wire.read()<<8 | Wire.read();

  // GyroOut is in the form GyroOut = Sensitivity*AngularRate. 
  RateRoll=(float)GyroX/65.5;
  RatePitch=(float)GyroY/65.5;
  RateYaw=(float)GyroZ/65.5;
}


void setup() {
  Serial.begin(57600);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Wire.setClock(400000);
  Wire.begin();
  delay(250);
  Wire.beginTransmission(0x68); 
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
}
void loop() {
  gyro_signals();
  Serial.print("Roll rate [deg/s]= ");
  Serial.print(RateRoll);
  Serial.print(" Pitch Rate [deg/s]= ");
  Serial.print(RatePitch);
  Serial.print(" Yaw Rate [deg/s]= ");
  Serial.println(RateYaw);
  delay(50);
}