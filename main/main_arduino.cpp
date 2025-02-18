#pragma once
#include "main_arduino.hpp"


const unsigned long BaudRate{57600};
const uint32_t ArdClock{400000};


void main_setup(void)
{
  Serial.begin(BaudRate);

  Serial.println("");
  Serial.println("Settting up Arduino ...");
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Wire.setClock(ArdClock);
  Wire.begin();
  delay(100);

  // Select Clock Source
  Wire.beginTransmission(I2C_ADD);
  Wire.write(PWR_MGMT_1);
  Wire.write(CLKSEL_20MHZ);
  Wire.endTransmission();

  IMU::InitImu();
}

  // TO_DO: Force calibration with long-press reset button
  

void main_loop(void)
{
  IMU::Axes angles1 = IMU::GetAccAngles();
  IMU::Axes angles2 = IMU::GetGyroAngles();
  IMU::Axes angles = IMU::ComplFilter(angles1, angles2, 0.04);
  Serial.print(F(" Acc_angle_Roll= "));
  Serial.print(angles.x);
  Serial.print(F(" Acc_angle_Pitch= "));
  Serial.println(angles.y);
}




