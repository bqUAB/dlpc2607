/*******************************************************************************
* Beaglebone configuration of DLPC2607 through I2C communication
* by Byron Quezada
*
* Configuration of the DLPC2607 to display the different Internal Test Patterns.
* This program stablishes the minimum configuration requirements to project a
* determined image using the DLP2000 LightCrafter EVM. The program implements
* I2C communication, and it shows how to write to the DLPC2607 registers by
* accessing them through the built-in I2C kernel driver in Debian. The I2C
* adapter is selected from userspace through the /dev interface.
* The i2c-dev module is needed but it's loaded by default.
*
* Created 31 October 2017.
*******************************************************************************/

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>         // Needed for unit uint8_t data type
#include <stdbool.h>        // Needed for bool function
#include <linux/i2c-dev.h>  // Needed to use the I2C Linux driver
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "dlpc2607.h"


bool DLPC2607_REG_WRITE(int file, uint8_t reg_add, uint8_t byte4,
                       uint8_t byte3, uint8_t byte2, uint8_t byte1){
  bool bSuccess = false;
  uint8_t w_buf[5];

  // Write to define register
  w_buf[0] = reg_add;
  w_buf[1] = byte4;
  w_buf[2] = byte3;
  w_buf[3] = byte2;
  w_buf[4] = byte1;
  if (write(file, &w_buf, sizeof(w_buf)) == sizeof(w_buf)){
    bSuccess = true;
  } else {
    /* ERROR HANDLING: i2c transaction failed */
    printf("Failed to write to the i2c bus.\n");
  }

  return bSuccess;
}

int main(){

  int file;
  uint8_t adapter_n = 2;  // Beaglebone I2C adapter
  char filename[13];
  bool bSuccess;

  printf("===== DLP Controller =====\n");
  printf("Internal Test Patterns demonstration\n");

  /* --------------------------> Open the I2C bus <-------------------------- */
  // Access an I2C bus from a C program
  snprintf(filename, 13, "/dev/i2c-%d", adapter_n);

  // Open the bus
  if ((file = open(filename, O_RDWR)) < 0) {
    /* ERROR HANDLING: you can check errno to see what went wrong */
    perror("Failed to open the I2C bus.");
    exit(1);
  }

  /* ----------------------> Initiating communication <---------------------- */
  // Specify device address to communicate
  uint8_t addr = 0x1B;  // DLPC2607 I2C address
  if (ioctl(file, I2C_SLAVE, addr) < 0) {
    printf("Failed to acquire bus access and/or talk to slave.\n");
    /* ERROR HANDLING; you can check errno to see what went wrong */
    exit(1);
  }

  /* -------------------------> Writing to DLPC2607 <------------------------- */
  // Initialization of the DLP
  bSuccess = DLPC2607_Init(file);

  uint8_t ct; // counter
  uint8_t reg_add = 0x11;  // Internal Test Patterns Select
  for(ct = 0; ct < 14; ct++){
    if(DLPC2607_REG_WRITE(file, reg_add, 0, 0, 0, ct)){
      printf("value = %X\n", ct);
    } else {
      printf("Failed to write to DLPC2607\n");
      break;
    }

    sleep(1);
  }

  return 0;
}
