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


  // Calibration: store or read from EEPROM
  Serial.println(F("Calibrating IMU ..."));
  int cal_addr = 0;
  ImuCal target;
  EEPROM.get(cal_addr, target);

  // If no values are stored, write new calibrations to EEPROM else read stored values.
  if (isnan(target.acccal.x)) 
  { 
    Serial.println(F("Calibration values in EEPROM address 0 is nan"));
    calibrateImu();
  } 
  else 
  {
    recalibrate();
    readCalibration();
  }
}

  // TO_DO: Force calibration with long-press reset button
  

void main_loop(void)
{
  Xyz angles1 = acc_angles(calibrations);
  Xyz angles2 = gyro_angles(calibrations);
  Xyz angles = compl_filter(angles1, angles2, 0.04);
  Serial.print(F(" Acc_angle_Roll= "));
  Serial.print(angles.x);
  Serial.print(F(" Acc_angle_Pitch= "));
  Serial.println(angles.y);
}


void calibrateImu() 
{
  Serial.println(F("Calibrating IMU ..."));
  calibrations = init_imu();
  EEPROM.put(0, calibrations);
  Serial.println(F("Calibration finished."));
  Serial.println(F("New values stored on EEPROM."));
}


void readCalibration()
{
  Serial.println(F("Reading stored cals from EEPROM."));
  EEPROM.get(0, calibrations);    
  delay(3000);
}


void recalibrate()
{
  Serial.println(F("Enter Y to recalibrate Gyro and Accelerometer:"));
  while (millis() < 5000)
  {
    if (Serial.available() ) 
    {
    int serinput = Serial.read();
    Serial.println((char)serinput);
      if ((char)serinput == 'Y')
      { 
        calibrateImu();
        break;
      } 
    }
  }
}

