#define ENABLE_GxEPD2_GFX 0
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <GxEPD2_7C.h>
#include <Fonts/FreeSansBold24pt7b.h>
#include <SHTSensor.h>
#include <Wire.h>
#include "Config.h"
#include "ESP32Helper.h"
#include "PersWiFiManager.h"

GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> display(GxEPD2_290(/*CS=*/ _CS_PIN, /*DC=*/ _DC_PIN, /*RST=*/ _RST_PIN, /*BUSY=*/ _BUSY_PIN));

SHTSensor sht(SHT_0x45);
float cTemp;

void setup() {
  Wire.begin(_SDA_PIN, _SCL_PIN);
  Serial.begin(115200);

  if (sht.init()) {
    sht.setAccuracy(SHTSensor::SHT_ACCURACY_HIGH);
  } else {
    Serial.println(F("SHT initialization failed"));
  }

  display.init(115200, true, 2, false);
  display.setPartialWindow(0, 0, display.width(), display.height());
}

void loop() {
  if (sht.readSample()) {
    cTemp = sht.getTemperature();
  }

  char buf[16];
  dtostrf(cTemp, 4, 1, buf);

  display.setRotation(2);
  display.setFont(&FreeSansBold24pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setPartialWindow(0, 0, display.width(), display.height());
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(0, FreeSansBold24pt7b.yAdvance);
    display.print(buf);
  }
  while (display.nextPage());
  delay(100);
}
