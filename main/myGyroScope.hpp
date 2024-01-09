#ifndef MY_GYRO_SCOPE_H_   
#define MY_GYRO_SCOPE_H_ 
#include <Wire.h>

extern float RateRoll, RatePitch, RateYaw;
extern int16_t GyroX, GyroY, GyroZ;

void gyro_signals(void);

#endif MY_GYRO_SCOPE_H_ 