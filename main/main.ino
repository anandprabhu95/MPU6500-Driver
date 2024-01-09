#include "myGyroScope.hpp"

float RateCalRoll, RateCalPitch, RateCalYaw;
int RateCalTime;

unsigned long BaudRate = 57600;
uint32_t ArdClock = 400000;

void setup() {
  Serial.begin(BaudRate);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Wire.setClock(ArdClock);
  Wire.begin();
  delay(250);

  // Select Clock Source
  Wire.beginTransmission(I2C_ADD);
  Wire.write(PWR_MGMT_1);
  Wire.write(CLKSEL_20MHZ);
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