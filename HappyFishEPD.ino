#define ENABLE_GxEPD2_GFX 0
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <GxEPD2_7C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <SHTSensor.h>
#include <Wire.h>
#include "Config.h"
#include "ESP32Helper.h"
#include "PersWiFiManager.h"

GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> display(GxEPD2_290(/*CS=*/ SS_PIN, /*DC=*/ DC_PIN, /*RST=*/ MISO_PIN, /*BUSY=*/ BUSY_PIN));

SHTSensor sht(SHT_0x45);
float cTemp;

void setup() {
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.begin(115200);

  display.init(115200);
  display.setPartialWindow(0, 0, display.width(), display.height());

  if (sht.init()) {
    sht.setAccuracy(SHTSensor::SHT_ACCURACY_HIGH);
  } else {
    Serial.println(F("SHT initialization failed"));
  }
}

void loop() {
  if (sht.readSample()) {
    cTemp = sht.getTemperature();
  }

  char buf[16];
  dtostrf(cTemp, 4, 1, buf);

  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby; uint16_t tbw, tbh;
  // align with centered HelloWorld
  display.getTextBounds(buf, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  // height might be different
  display.getTextBounds(buf, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t y = ((display.height() / 4) - tbh / 2) - tby; // y is base line!
  // make the window big enough to cover (overwrite) descenders of previous text
  uint16_t wh = FreeMonoBold9pt7b.yAdvance;
  uint16_t wy = (display.height() / 4) - wh / 2;
  display.setPartialWindow(0, wy, display.width(), wh);
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x, y);
    display.print(buf);
  }
  while (display.nextPage());

  delay(100);
}
