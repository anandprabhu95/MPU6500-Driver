#include "main_arduino.hpp"
#include "imu.hpp"

static ImuCal bb;

void setup() {
  main_setup();
  ImuCal bb = init_imu();
}

void loop() {
  Xyz gres = gyro_signals();
  Xyz ares = acc_signals();

//  gres.x = gres.x - zz.gyrocal.x;
//  gres.y = gres.y - zz.gyrocal.y;
//  gres.z = gres.z - zz.gyrocal.z;
//
//  ares.x = ares.x - zz.acccal.x;
//  ares.y = ares.y - zz.acccal.y;
//  ares.z = ares.z - zz.acccal.z;

  //Serial.print(" Roll rate [deg/s]= ");
  //Serial.print(gres.x);
  //Serial.print(" Pitch Rate [deg/s]= ");
  //Serial.print(gres.y);
  //Serial.print(" Yaw Rate [deg/s]= ");
  //Serial.print(gres.z);
//
  //Serial.print(" aX [g]= ");
  //Serial.print(ares.x);
  //Serial.print(" aY [g]= ");
  //Serial.print(ares.y);
  //Serial.print(" aZ [g]= ");
  //Serial.print(ares.z);
  //Serial.println(a);

  Xyz thres = acc_angles(bb);
  Serial.print(" aX [g]= ");
  Serial.print(thres.x);
  Serial.print(" aY [g]= ");
  Serial.println(thres.y);

  delay(sample_rate);
}