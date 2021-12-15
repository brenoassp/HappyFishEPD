#define ENABLE_GxEPD2_GFX 0
#include <Button2.h>
#include <GxEPD2_3C.h>
#include <GxEPD2_7C.h>
#include <GxEPD2_BW.h>
#include <LittleFS.h>
#include <SHTSensor.h>
#include <ThingSpeak.h>
#include <TimeAlarms.h>
#include <TimeLib.h>
#include <U8g2_for_Adafruit_GFX.h>
#include <Wire.h>
#include "Config.h"
#include "ESP32Helper.h"
#include "PersWiFiManager.h"

unsigned long myChannelNumber = 1592680;
const char*   myWriteAPIKey = "N9VTLDZOJEQC2FQX";

GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> display(GxEPD2_290(/*CS=*/ _CS_PIN, /*DC=*/ _DC_PIN, /*RST=*/ _RST_PIN, /*BUSY=*/ _BUSY_PIN));
U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;

Button2 btn1(_D36_PIN);
Relay r1(_D38_PIN);
Relay r2(_D40_PIN);
Tstat tstat(r2);

SHTSensor sht(SHT_0x44);
float cTemp;

bool shouldReboot = false;
PersWiFiManager persWM;
WiFiClient client;

TaskHandle_t xHandle;

void setup() {
  Serial.begin(115200);
  // while (!Serial) ;
  Wire.begin(_SDA_PIN, _SCL_PIN);
  xTaskCreatePinnedToCore(vTaskCode, "vTaskCode", 2048, NULL,1, &xHandle, /* core */ 0);

  if (sht.init()) {
    sht.setAccuracy(SHTSensor::SHT_ACCURACY_HIGH);
    tstat.setup(TstatMode::HEAT,
                /* setpoint */   25.5,
                /* hysteresis */ 1,
                /* lowerLimit */ 0,
                /* upperLimit */ 28,
                /* interval */   timeoutLength*10);
  } else {
    Serial.println(F("SHT initialization failed"));
  }

  display.init();
  u8g2Fonts.begin(display);
  display.setRotation(1);

  r1.setup();
  r2.setup();
  digitalWrite(btn1.getAttachPin(), LOW);
  btn1.setPressedHandler([](Button2& btn) { r1.toggle(); });

  if (!LittleFS.begin()) {
    Serial.println(F("Failed to mount file system"));
  }
  loadConfigFile(configFilePath, config);
  persWM.begin();

  setSyncProvider([](){ return rtc.getEpoch(); });
  Alarm.alarmRepeat(0,0,config.alarms.on, [](){ r1.turnOn(); });
  Alarm.alarmRepeat(0,0,config.alarms.off, []() { r1.turnOff(); });

  ThingSpeak.begin(client);
}

void loop() {
  if (shouldReboot) {
    Serial.println(F("Rebooting..."));
    delay(100);
    ESP.restart();
  }
  Alarm.delay(1000);
  persWM.handleWiFi();

  if (sht.readSample()) {
    cTemp = sht.getTemperature();
    tstat.handle(cTemp);
  }

  display.setRotation(1);
  u8g2Fonts.setFontMode(1);
  u8g2Fonts.setFontDirection(0);
  u8g2Fonts.setFont(u8g2_font_logisoso46_tf);
  char buf[16];
  snprintf_P(buf,
             sizeof(buf),
             PSTR("%.3f°"),
             cTemp);
  middlePartialUpdate(buf);

  ThingSpeak.setField(1, cTemp);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
}

void vTaskCode(void* pvParameter) {
  for(;;) {
    btn1.loop();
  }
}

void middlePartialUpdate(const char* buf) {
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
