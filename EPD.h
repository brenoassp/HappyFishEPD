#ifndef _EPD_H_
#define _EPD_H_

#define ENABLE_GxEPD2_GFX 0

#include <Arduino.h>
#include <GxEPD2_3C.h>
#include <GxEPD2_7C.h>
#include <GxEPD2_BW.h>
#include <U8g2_for_Adafruit_GFX.h>
#include "ESP32Helper.h"

class EPDClass {
  public:
    void setup();
    void partialUpdate(const char* buf);
};

extern EPDClass EPD;
extern GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> Display;
extern U8G2_FOR_ADAFRUIT_GFX U8g2Fonts;

#endif
