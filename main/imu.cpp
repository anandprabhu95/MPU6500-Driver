#pragma once
#include "imu.hpp"

Xyz gyro_signals(void)
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
  Xyz gyro;
  gyro.x = static_cast<float>(GyroX) / 65.5;
  gyro.y = static_cast<float>(GyroY) / 65.5;
  gyro.z = static_cast<float>(GyroZ) / 65.5;
  return gyro;
}


Xyz acc_signals(void)
{
  //  The Gyro Output is a 16 bit value transmitted as two 8 bit values
  //  Concatonate two 8 bit values to get the 16 bit value.

  Xyz acc;
  Wire.beginTransmission(I2C_ADD);
  Wire.write(ACCEL_XOUT_H);
  Wire.endTransmission();
  Wire.requestFrom(I2C_ADD, 6);
  acc.x = (float)(Wire.read() << 8 | Wire.read()) / 4096.0;
  acc.y = (float)(Wire.read() << 8 | Wire.read()) / 4096.0;
  acc.z = (float)(Wire.read() << 8 | Wire.read()) / 4096.0;
  return acc;
}


ImuCal init_imu(void)
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
 
  ImuCal cals{0};

  for (RateCalTime = 0; RateCalTime < 10000; RateCalTime++) 
  {
    Xyz gyrosig = gyro_signals();
    
    cals.gyrocal.x = cals.gyrocal.x + gyrosig.x;
    cals.gyrocal.y = cals.gyrocal.y + gyrosig.y;
    cals.gyrocal.z = cals.gyrocal.z + gyrosig.z;
    delay(1);
  }
  cals.gyrocal.x /= 10000;
  cals.gyrocal.y /= 10000;
  cals.gyrocal.z /= 10000;

  cals.acccal.x = 0.09;
  cals.acccal.y = 0.051;
  cals.acccal.z = 0.03; 
  return cals;
}


Xyz gyro_angles(ImuCal calval)
{
  gyrorateForAngles = gyro_signals();
  gyrorateForAngles.x = gyrorateForAngles.x - calval.gyrocal.x;
  gyrorateForAngles.y = gyrorateForAngles.y - calval.gyrocal.y;
  gyrorateForAngles.z = gyrorateForAngles.z - calval.gyrocal.z;
  gyro_theta.x = (gyro_theta.x + gyrorateForAngles.x*0.004);
  gyro_theta.y = (gyro_theta.y + gyrorateForAngles.y*0.004);
  gyro_theta.z = (gyro_theta.z + gyrorateForAngles.z*0.004);
  return gyro_theta;
}


Xyz acc_angles(ImuCal calval)
{
  accForAngles = acc_signals();
  accForAngles.x = accForAngles.x - calval.acccal.x;
  accForAngles.y = accForAngles.y - calval.acccal.y;
  accForAngles.z = accForAngles.z - calval.acccal.z;
  acc_theta.x =  atan(accForAngles.y / sqrt(accForAngles.x*accForAngles.x+accForAngles.z*accForAngles.z)) * 1/(3.142/180);
  acc_theta.y = -atan(accForAngles.x / sqrt(accForAngles.y*accForAngles.y+accForAngles.z*accForAngles.z)) * 1/(3.142/180);
  //acc_theta.z = 0.0;
  return acc_theta;
}


// Complementary filter 
Xyz compl_filter(Xyz acc_meas, Xyz gyr_meas, float filter_gain)
{
  filtered_val.x = filter_gain*gyr_meas.x + (1-filter_gain)*acc_meas.x;
  filtered_val.y = filter_gain*gyr_meas.y + (1-filter_gain)*acc_meas.y;
  filtered_val.z = filter_gain*gyr_meas.z + (1-filter_gain)*acc_meas.z;
  return filtered_val;
} 

// TO-DO: Use Kalman filter instead