#include "EPD.h"

EPDClass EPD;
GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> display(GxEPD2_290(_CS_PIN, _DC_PIN, _RST_PIN, _BUSY_PIN));
U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;

void EPDClass::setup() {
  display.init();
  u8g2Fonts.begin(display);
  display.setRotation(1);
}

void EPDClass::partialUpdate(const char *buf) {
  u8g2Fonts.setFontMode(1);
  u8g2Fonts.setFontDirection(0);
  uint16_t bg = GxEPD_WHITE;
  uint16_t fg = GxEPD_BLACK;
  u8g2Fonts.setForegroundColor(fg);
  u8g2Fonts.setBackgroundColor(bg);
  int16_t tw = u8g2Fonts.getUTF8Width(buf);
  int16_t ta = u8g2Fonts.getFontAscent();
  int16_t td = u8g2Fonts.getFontDescent();
  int16_t th = ta - td;
  uint16_t x = (display.width() - tw) / 2;
  uint16_t y = (display.height() - th) / 2 + ta;
  display.setPartialWindow(0, 0, display.width(), display.height());
  display.firstPage();
  do
  {
    display.fillScreen(bg);
    u8g2Fonts.setCursor(x, y);
    u8g2Fonts.print(buf);
  }
  while (display.nextPage());
  delay(100);
}