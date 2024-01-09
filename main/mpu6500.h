// Register Map Source: https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6500-Register-Map2.pdf

#ifndef _MPU6500_H_
#define _MPU6500_H_

#include <Wire.h>

#define I2C_ADD               0x68
#define PWR_MGMT_1            0x6B
#define CLKSEL_20MHZ          0x00
#define CONFIG                0x1A
#define DLFP_CONFIG_10HZ      0x05
#define GYRO_CONFIG           0x1B
#define GYRO_FS_SEL           0x08  //= ±500dps
#define GYRO_XOUT_H           0x43 
 

#endif 