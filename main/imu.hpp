#pragma once
#include "mpu6500.h"
#include "Arduino.h"
#include "EEPROM.h"


namespace IMU
{
  static int16_t GyroX, GyroY, GyroZ;
  static int16_t RateCalTime;

  struct Axes 
  {
    float x, y, z;
  };

  struct ImuCal 
  {
    Axes gyroCal;
    Axes accCal;
  };

  static ImuCal calibrations{0};

  static Axes gyroTheta{0};
  static Axes accTheta{0};
  static Axes gyrorateForAngles{0};
  static Axes accForAngles{0};
  static Axes filteredVal{0};

  
  Axes GetGyroSignals(void);
  Axes GetAccSignals(void);
  extern void InitImu(void);

  void CalibrateImu(void);
  void ReadCalibration(void);
  void Recalibrate(void);

  extern Axes GetGyroAngles(void);
  extern Axes GetAccAngles(void);
  extern Axes ComplFilter(Axes acc_meas, Axes gyr_meas, float filter_gain);
}
