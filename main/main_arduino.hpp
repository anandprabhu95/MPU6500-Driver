#pragma once
#include "Arduino.h"
#include "mpu6500.h"
#include "imu.hpp"
#include "EEPROM.h"

static ImuCal calibrations;

// Arduino setup() and loop() will be defined here in the future
void main_setup(void);
void main_loop(void);

