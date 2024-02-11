#include "main_arduino.hpp"
#include "imu.hpp"


ImuCal zz;

void setup() {
  main_setup();
  Serial.println(zz.gyrocal.x);
  zz = init_imu();
  delay(10);
}

void loop() {
  //Xyz gres = gyro_signals();
  //Xyz ares = acc_signals();

  //gres.x = gres.x - zz.gyrocal.x;
  //gres.y = gres.y - zz.gyrocal.y;
  //gres.z = gres.z - zz.gyrocal.z;
//
  //ares.x = ares.x - zz.acccal.x;
  //ares.y = ares.y - zz.acccal.y;
  //ares.z = ares.z - zz.acccal.z;
//
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
  //Serial.println(ares.z);


  Xyz thres = acc_angles(zz);
  Serial.print(" aX [g]= ");
  Serial.print(thres.x);
  Serial.print(" aY [g]= ");
  Serial.println(thres.y);

  delay(sample_rate);
}