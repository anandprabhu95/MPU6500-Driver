#include "myGyroScope.hpp"

float RateCalRoll, RateCalPitch, RateCalYaw;
int RateCalTime;

void setup() {
  Serial.begin(57600);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Wire.setClock(400000);
  Wire.begin();
  delay(250);
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);  //PWR_MGMT_1
  Wire.write(0x00);  //CLKSEL == 0 : Clock Source: Internal 20MHz oscillator
  Wire.endTransmission();


  // GyroScope Calibration
  // Take measurements gor 2000 ms and sum the measurements
  for (RateCalTime = 0;
       RateCalTime < 2000;
       RateCalTime++) {
    gyro_signals();
    RateCalRoll = RateCalRoll + RateRoll;
    RateCalPitch = RateCalPitch + RatePitch;
    RateCalYaw = RateCalYaw + RateYaw;
    delay(1);
  }
  // Average the measurements to obtain calibration values
  RateCalRoll /= 2000;
  RateCalPitch /= 2000;
  RateCalYaw /= 2000;
}


void loop() {
  gyro_signals();
  RateRoll = RateRoll - RateCalRoll;
  RatePitch = RatePitch - RateCalPitch;
  RateYaw = RateYaw - RateCalYaw;
  Serial.print("Roll rate [deg/s]= ");
  Serial.print(RateRoll);
  Serial.print(" Pitch Rate [deg/s]= ");
  Serial.print(RatePitch);
  Serial.print(" Yaw Rate [deg/s]= ");
  Serial.println(RateYaw);
  delay(50);
}