/*
Header file for the DLPC2607 coupled with a DLP2000.
Basic functionality
*/

#include <stdio.h>
#include <stdint.h>   // Needed for unit uint8_t data type
#include <stdbool.h>  // Needed for bool function
#include <time.h>     // Needed for timespec
#include "i2c.h"

#ifndef DLPC2607_H_
#define DLPC2607_H_

const uint8_t kDlpc2607Addr = 0x36;
                                 // Reset value
const uint8_t kInSrcSel = 0x0B;  // 0x02  Splash
const uint8_t kInResSel = 0x0C;  // 0x01  QVGA landscape
const uint8_t kPxDataFmtSel = 0x0D;  // 0x02 RGB888

const uint8_t kRgbLedDrvEn = 0x16;  // 0x00 disabled

const uint8_t kSoftRst = 0x1F;  // 0x00 Not in reset

const uint8_t kDmdParkCtrl = 0x2D;  // 0x00 Disabled

class Dlpc2607 {
  protected:
    I2cBus* ptr_i2c;

    enum InSrc {
      // Input Source
      kParallelItf = 0,  // Parallel interface
      kIntTestPattern = 1,
      kSplashScrn = 2,
      // 3 is reserved
      kBt656Itf = 4
    };

    enum PxDataFmt {
      // Pixel Data Format
      // Check datasheet if YCrCb is needed
      kRgb565Bus16 = 0,  // RGB565 transferred on a 16-bit bus
      kRgb666Bus18 = 1,
      kRgb888Bus24 = 2,
      kRgb565Bus8 = 3,
      kRgb888Bus8 = 4,
      kRgb888Bus16 = 5,
      kRgb666Bus8 = 6,
      kRgb666Bus16 = 7
    };

    enum InRes {
      // Supported Input and Output Resolutions for the 0.2 nHD DMD (DLP2000)
      // P = portrait, L = landscape
      kQvgaP = 0,
      kQvgaL = 1,
      kQwvgaP = 2,
      kQwvgaL = 3,
      k23VgaP = 4,  // 2:3
      k32VgaL = 5,  // 3:2
      kVgaP = 6,
      kVgaL = 7,
      kWvga720P = 8,
      kWvga720L = 9,
      kWvga752P = 10,
      kWvga752L = 11,
      kWvga800P = 12,
      kWvga800L = 13,
      kWvga852P = 14,
      kWvga852L = 15,
      kWvga853P = 16,
      kWvga853L = 17,
      kWvga854P = 18,
      kWvga854L = 19,
      kWvga864P = 20,
      kWvga864L = 21
      // 22 is reserved
      kNtsc = 23,  // landscape
      // 24 is reserved
      kPal = 25,  // landscape
      kNhdP = 26,
      kNhdL = 27,
      // 28 is reserved
      kNhdLVga = 29
    };

    // Set initial input paramaters

  public:
    uint8_t DmdPark(bool park);
    uint8_t SoftRst();
    uint8_t RgbLedDrvEn(bool red, bool green, bool blue);
}

// Functions
bool DLPC2607_Init(int file);

#endif  /*DLPC2607_H_*/