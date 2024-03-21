#pragma once
#include "mpu6500.h"
#include "Arduino.h"

static int16_t GyroX, GyroY, GyroZ;
static int16_t RateCalTime;

struct Xyz {
  float x, y, z;
};

struct Xyz2 {
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

static Xyz filtered_val{0};

Xyz compl_filter(Xyz acc_meas, Xyz gyr_meas, float filter_gain);
