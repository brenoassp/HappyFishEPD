#include "EPD.h"

EPDClass EPD;
GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> Display(GxEPD2_290(_CS_PIN, _DC_PIN, _RST_PIN, _BUSY_PIN));
U8G2_FOR_ADAFRUIT_GFX U8g2Fonts;

void EPDClass::setup() {
  Display.init();
  U8g2Fonts.begin(Display);
  Display.setRotation(1);
}

void EPDClass::partialUpdate(const char* buf) {
  U8g2Fonts.setFontMode(1);
  U8g2Fonts.setFontDirection(0);
  uint16_t bg = GxEPD_WHITE;
  uint16_t fg = GxEPD_BLACK;
  U8g2Fonts.setForegroundColor(fg);
  U8g2Fonts.setBackgroundColor(bg);
  int16_t tw = U8g2Fonts.getUTF8Width(buf);
  int16_t ta = U8g2Fonts.getFontAscent();
  int16_t td = U8g2Fonts.getFontDescent();
  int16_t th = ta - td;
  uint16_t x = (Display.width() - tw) / 2;
  uint16_t y = (Display.height() - th) / 2 + ta;
  Display.setPartialWindow(0, 0, Display.width(), Display.height());
  Display.firstPage();
  do
  {
    Display.fillScreen(bg);
    U8g2Fonts.setCursor(x, y);
    U8g2Fonts.print(buf);
  }
  while (Display.nextPage());
  delay(100);
}