#pragma once
#include "mpu6500.h"
#include "Arduino.h"

static int16_t GyroX, GyroY, GyroZ;
static int RateCalTime;
const uint8_t sample_rate{10};

struct Xyz {
  float x, y, z;
};

struct ImuCal {
  Xyz gyrocal;
  Xyz acccal;
};

static Xyz gyro_theta{0};
static Xyz acc_theta{0};
static Xyz gyrorateForAngles{0};
static Xyz accForAngles{0};

Xyz gyro_signals(void);
Xyz acc_signals(void);
ImuCal init_imu(void);
Xyz gyro_angles(ImuCal calval);
Xyz acc_angles(ImuCal calval);
