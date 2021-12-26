#include "EPD.h"

EPDClass EPD;

void EPDClass::setup() {
  _display.init();
  _u8g2Fonts.begin(_display);
  _display.setRotation(1);
}

U8G2_FOR_ADAFRUIT_GFX* EPDClass::getFonts() {
  return &_u8g2Fonts;
}

void EPDClass::partialUpdate(const char* buf) {
  _u8g2Fonts.setFontMode(1);
  _u8g2Fonts.setFontDirection(0);
  uint16_t bg = GxEPD_WHITE;
  uint16_t fg = GxEPD_BLACK;
  _u8g2Fonts.setForegroundColor(fg);
  _u8g2Fonts.setBackgroundColor(bg);
  int16_t tw = _u8g2Fonts.getUTF8Width(buf);
  int16_t ta = _u8g2Fonts.getFontAscent();
  int16_t td = _u8g2Fonts.getFontDescent();
  int16_t th = ta - td;
  uint16_t x = (_display.width() - tw) / 2;
  uint16_t y = (_display.height() - th) / 2 + ta;
  _display.setPartialWindow(0, 0, _display.width(), _display.height());
  _display.firstPage();
  do
  {
    _display.fillScreen(bg);
    _u8g2Fonts.setCursor(x, y);
    _u8g2Fonts.print(buf);
  }
  while (_display.nextPage());
  delay(100);
}