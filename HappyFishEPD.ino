#define ENABLE_GxEPD2_GFX 0
#include <GxEPD2_3C.h>
#include <GxEPD2_7C.h>
#include <GxEPD2_BW.h>
#include <SHTSensor.h>
#include <U8g2_for_Adafruit_GFX.h>
#include <Wire.h>
#include "Config.h"
#include "ESP32Helper.h"
#include "PersWiFiManager.h"

GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> display(GxEPD2_290(/*CS=*/ _CS_PIN, /*DC=*/ _DC_PIN, /*RST=*/ _RST_PIN, /*BUSY=*/ _BUSY_PIN));
U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;

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

  display.init(115200);
  u8g2Fonts.begin(display);
}

void loop() {
  if (sht.readSample()) {
    cTemp = sht.getTemperature();
  }
  partialUpdate();
}

void partialUpdate() {
  char buf[16];
  dtostrf(cTemp, 4, 1, buf);

  uint16_t bg = GxEPD_WHITE;
  uint16_t fg = GxEPD_BLACK;
  display.setRotation(2);
  u8g2Fonts.setFontMode(1);
  u8g2Fonts.setFontDirection(0);
  u8g2Fonts.setForegroundColor(fg);
  u8g2Fonts.setBackgroundColor(bg);
  u8g2Fonts.setFont(u8g2_font_logisoso42_tf);
  int16_t tw = u8g2Fonts.getUTF8Width(buf);
  int16_t ta = u8g2Fonts.getFontAscent();
  int16_t td = u8g2Fonts.getFontDescent();
  int16_t th = ta - td;
  uint16_t x = (display.width() - tw) / 2; // middle
  display.setPartialWindow(0, 0, display.width(), th+10);
  display.firstPage();
  do
  {
    display.fillScreen(bg);
    u8g2Fonts.setCursor(0, ta+10);
    u8g2Fonts.print(buf);
    u8g2Fonts.setFont(u8g2_font_logisoso24_tf);
    u8g2Fonts.setCursor(tw, 38);
    u8g2Fonts.print("°C");
  }
  while (display.nextPage());
  delay(500);
}