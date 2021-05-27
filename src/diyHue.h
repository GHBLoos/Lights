#ifndef _diyhue_H_
#define _diyhue_H_
#include <Arduino.h>


enum Led {
    kLedCCT = 2,
    kLedRGB = 3,
    kLedRGBW = 4
};

struct ledRGBW {
  uint8_t channels=kLedRGBW;
  uint8_t colors[kLedRGBW];
};

struct currentledRGBW {
  float channels=kLedRGBW;
  float colors[kLedRGBW];
  float stepLevel[kLedRGBW];
};


#endif
