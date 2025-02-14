#pragma once
#include "main_arduino.hpp"

int main(void)
{ 
  init();
  main_setup();

  while(true)
  {
    main_loop();
  }
}