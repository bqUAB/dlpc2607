// ***************************************************************************
// Linux ARM configuration of DLPC2607 through I2C communication
// by Byron Quezada
//
// Configuration of the DLPC2607 to display the different Internal Test
// Patterns. This program stablishes the minimum configuration requirements to
// project a determined image using the DLP2000 LightCrafter EVM. The program
// implements I2C communication, and it shows how to write to the DLPC2607
// registers by accessing them through the built-in I2C kernel driver in
// Debian. The I2C adapter is selected from userspace through
// the /dev interface. The i2c-dev module is needed but it's loaded by
// default.
//
// Created 31 October 2017.
// ***************************************************************************

#include <stdio.h>
#include <stdint.h>  // Needed for unit uint8_t data type
#include "i2c.h"
#include "dlpc2607.h"


int main(){
  I2cBus i2c_bus(1);
  Dlpc2607 dlp(&i2c_bus);

  printf("===== DLP Controller =====\n");
  printf("Internal Test Patterns demonstration\n");

  // Initialization of the DLP
  if (dlp.Init()) {
    printf("DLP Initialization Successful!\n");
  }

  uint16_t addr = 0x1B;  // DLPC2607 address
  uint8_t reg = 0x11;  // Internal test pattern select
  uint8_t value_in[4] = {0};  // Pattern

  // Internal test patterns looping
  for(uint8_t i = 0; i < 0x0E; i++){
    value_in[3] = i;

    if(i2c_bus.WriteToMemFrom(addr, reg, 4, value_in)) {
      printf("pattern = %X\n", i);
    } else {
      printf("Failed to write to DLPC2607\n");
      break;
    }

    sleep(1);
  }

  return 0;
}
