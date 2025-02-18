#pragma once
#include "imu.hpp"

namespace IMU{

Axes GetGyroSignals(void)
{
  //  The Gyro Output is a 16 bit value transmitted as two 8 bit values
  //  Concatonate two 8 bit values to get the 16 bit value.
  Wire.beginTransmission(I2C_ADD);
  Wire.write(GYRO_XOUT_H);
  Wire.endTransmission();
  Wire.requestFrom(I2C_ADD, 6);
  GyroX = Wire.read() << 8 | Wire.read();
  GyroY = Wire.read() << 8 | Wire.read();
  GyroZ = Wire.read() << 8 | Wire.read();
  
  // GyroOut is in the form GyroOut = Sensitivity*AngularRate.
  Axes gyro;
  gyro.x = static_cast<float>(GyroX) / 65.5;
  gyro.y = static_cast<float>(GyroY) / 65.5;
  gyro.z = static_cast<float>(GyroZ) / 65.5;
  return gyro;
}


Axes GetAccSignals(void)
{
  //  The Gyro Output is a 16 bit value transmitted as two 8 bit values
  //  Concatonate two 8 bit values to get the 16 bit value.

  Axes acc;
  Wire.beginTransmission(I2C_ADD);
  Wire.write(ACCEL_XOUT_H);
  Wire.endTransmission();
  Wire.requestFrom(I2C_ADD, 6);
  acc.x = (float)(Wire.read() << 8 | Wire.read()) / 4096.0;
  acc.y = (float)(Wire.read() << 8 | Wire.read()) / 4096.0;
  acc.z = (float)(Wire.read() << 8 | Wire.read()) / 4096.0;
  return acc;
}

void InitImu(void) 
{
  // Calibration: store or read from EEPROM
  int cal_addr = 0;
  ImuCal target;
  EEPROM.get(cal_addr, target);

  // If no values are stored, write new calibrations to EEPROM else read stored values.
  if (isnan(target.accCal.x)) 
  { 
    Serial.println(F("Calibration values in EEPROM address 0 is nan"));
    CalibrateImu();
  } 
  else 
  {
    Recalibrate();
    ReadCalibration();
  }
}


void ReadCalibration(void)
{
  Serial.println(F("Reading stored cals from EEPROM."));
  EEPROM.get(0, calibrations);    
  delay(3000);
}


void Recalibrate(void)
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
        CalibrateImu();
        break;
      } 
    }
  }
}


void CalibrateImu(void)
{
  // Choose Low-Pass filter with a cutoff of 10Hz.
  Wire.beginTransmission(I2C_ADD);
  Wire.write(CONFIG);
  Wire.write(DLFP_CONFIG_10HZ);
  Wire.endTransmission();

  // Choose LSB Sensititivity of the gyroscope 65.5 LSB/deg/s.
  Wire.beginTransmission(I2C_ADD);
  Wire.write(GYRO_CONFIG);
  Wire.write(GYRO_FS_SEL);
  Wire.endTransmission();

  // Choose LSB Sensititivity of the accelerometer 4096 LSB/deg/s.
  Wire.beginTransmission(I2C_ADD);
  Wire.write(ACCEL_CONFIG);
  Wire.write(ACCEL_FS_SEL);
  Wire.endTransmission();

  // GyroScope Calibration
  // Take measurements for RateCalTime(ms) and average the measurements to obtain calibration values
  Serial.println(F("Calibrating IMU ..."));
  ImuCal cals{0};

  for (RateCalTime = 0; RateCalTime < 10000; RateCalTime++) 
  {
    Axes gyrosig = IMU::GetGyroSignals();
    
    calibrations.gyroCal.x = calibrations.gyroCal.x + gyrosig.x;
    calibrations.gyroCal.y = calibrations.gyroCal.y + gyrosig.y;
    calibrations.gyroCal.z = calibrations.gyroCal.z + gyrosig.z;
    delay(1);
  }
  calibrations.gyroCal.x /= 10000;
  calibrations.gyroCal.y /= 10000;
  calibrations.gyroCal.z /= 10000;

  calibrations.accCal.x = 0.09;
  calibrations.accCal.y = 0.051;
  calibrations.accCal.z = 0.03;
  Serial.println(F("Calibration finished."));
}


Axes GetGyroAngles(void)
{
  gyrorateForAngles = IMU::GetGyroSignals();
  gyrorateForAngles.x = gyrorateForAngles.x - calibrations.gyroCal.x;
  gyrorateForAngles.y = gyrorateForAngles.y - calibrations.gyroCal.y;
  gyrorateForAngles.z = gyrorateForAngles.z - calibrations.gyroCal.z;
  gyroTheta.x = (gyroTheta.x + gyrorateForAngles.x*0.004);
  gyroTheta.y = (gyroTheta.y + gyrorateForAngles.y*0.004);
  gyroTheta.z = (gyroTheta.z + gyrorateForAngles.z*0.004);
  return gyroTheta;
}


Axes GetAccAngles(void)
{
  accForAngles = IMU::GetAccSignals();
  accForAngles.x = accForAngles.x - calibrations.accCal.x;
  accForAngles.y = accForAngles.y - calibrations.accCal.y;
  accForAngles.z = accForAngles.z - calibrations.accCal.z;
  accTheta.x =  atan(accForAngles.y / sqrt(accForAngles.x*accForAngles.x+accForAngles.z*accForAngles.z)) * 1/(3.142/180);
  accTheta.y = -atan(accForAngles.x / sqrt(accForAngles.y*accForAngles.y+accForAngles.z*accForAngles.z)) * 1/(3.142/180);
  //accTheta.z = 0.0;
  return accTheta;
}


// Complementary filter 
Axes ComplFilter(Axes acc_meas, Axes gyr_meas, float filter_gain)
{
  filteredVal.x = filter_gain*gyr_meas.x + (1-filter_gain)*acc_meas.x;
  filteredVal.y = filter_gain*gyr_meas.y + (1-filter_gain)*acc_meas.y;
  filteredVal.z = filter_gain*gyr_meas.z + (1-filter_gain)*acc_meas.z;
  return filteredVal;
} 
}
// TO-DO: Use Kalman filter instead