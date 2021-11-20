#define ENABLE_GxEPD2_GFX 0
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <GxEPD2_7C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Wire.h>
#include "Config.h"
#include "ESP32Helper.h"
#include "PersWiFiManager.h"

GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> display(GxEPD2_290(/*CS=*/ SS_PIN, /*DC=*/ DC_PIN, /*RST=*/ MISO_PIN, /*BUSY=*/ BUSY_PIN));

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  display.init();
  // comment out next line to have no or minimal Adafruit_GFX code
  display.setTextColor(GxEPD_BLACK);
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    // comment out next line to have no or minimal Adafruit_GFX code
    display.print("Hello World!");
  }
  while (display.nextPage());
}

void loop() {

}
