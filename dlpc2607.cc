/*
Source file for the DLPC2607 coupled with a DLP2000 with basic functionality
*/

#include "dlpc2607.h"

// Dlpc2607 constructor
Dlpc2607::Dlpc2607(I2cBus* i2c_n) {
  ptr_i2c = i2c_n;
}

uint8_t Dlpc2607::DmdPark(bool park) {
  if (park) {
    ptr_i2c->WriteToMem(kDlpc2607Addr, kDmdParkCtrl, 0x01);
  } else {
    ptr_i2c->WriteToMem(kDlpc2607Addr, kDmdParkCtrl, 0x00);
  }
}

uint8_t Dlpc2607::SoftRst() {
  // Bit value is don't care so a 1 or 0 can be issued.
  // Prior to issuing a software reset command, TI recommends that a DMD PARK
  // (I2C: 0x2D) command be applied first, followed by a 500-μs wait interval
  // before issuing the reset.
  ptr_i2c->WriteToMem(kDlpc2607Addr, kDmdParkCtrl, 0x01);
}

uint Dlpc2607::RgbLedDrvEn(bool red, bool green, bool blue) {
  uint8_t value = 0;
  bool rgb[3] = {red, green, blue};

  for (int i = 0; i < 3; x++) {
    if (rgb[i]) {
      value |= 1 << i;
    } else {
      value |= 0 << i;
    }
  }

  ptr_i2c->WriteToMem(kDlpc2607Addr, kRgbLedDrvEn, value);
}

// API for register access, defined in main.c
bool DLPC2607_REG_WRITE(int file, uint8_t reg_add, uint8_t byte4,
                       uint8_t byte3, uint8_t byte2, uint8_t byte1);

bool DLPC2607_Init(int file){

  bool bSuccess;

  // A DMD PARK is recommended before a Software Reset.
  bSuccess = DLPC2607_REG_WRITE(file, DLPC2607_REG_DMD_PARK, \
                                0, 0, 0, DLPC_PARK);

  // Recommended time to sleep
  int microsec = 500;  // in microsenconds
  struct timespec req = {0};
  req.tv_sec = 0;
  req.tv_nsec = microsec * 1000L;
  nanosleep(&req, (struct timespec *)NULL);

  // Software Reset
  if(bSuccess){
    bSuccess = DLPC2607_REG_WRITE(file, DLPC2607_REG_SOFT_RST, \
                                  0, 0, 0, DLPC_RST);
    sleep(1);
  }

  // Input Source Selection
  if(bSuccess){
    bSuccess = DLPC2607_REG_WRITE(file, DLPC2607_REG_IN_SRC, \
                                  0, 0, 0, DLPC_INT_TST_PAT);
  }

  // Set resolution
  if(bSuccess){
    bSuccess = DLPC2607_REG_WRITE(file, DLPC2607_REG_IN_RES, \
                                  0, 0, 0, DLPC_NHDL);
  }

  // Turn ON LEDs
  if(bSuccess){
    bSuccess = DLPC2607_REG_WRITE(file, DLPC2607_REG_LED_EN,\
                                  0, 0, 0, \
                                  DLPC_RED_EN | DLPC_GREEN_EN | DLPC_BLUE_EN);
  }

}
