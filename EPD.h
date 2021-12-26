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
  GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> _display;
  U8G2_FOR_ADAFRUIT_GFX                     _u8g2Fonts;

  public:
    EPDClass():
              _display(GxEPD2_290(_CS_PIN, _DC_PIN, _RST_PIN, _BUSY_PIN))
              {};
    void setup();
    U8G2_FOR_ADAFRUIT_GFX* getFonts();
    void partialUpdate(const char* buf);
};

extern EPDClass EPD;

#endif
