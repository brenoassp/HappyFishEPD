#include <Button2.h>
#include <LittleFS.h>
#include <SHTSensor.h>
#include <ThingSpeak.h>
#include <TimeAlarms.h>
#include <TimeLib.h>
#include <Wire.h>
#include "ESP32Helper.h"
#include "Config.h"
#include "EPD.h"
#include "PersWiFiManager.h"

unsigned long myChannelNumber = 1592680;
const char*   myWriteAPIKey = "N9VTLDZOJEQC2FQX";

WiFiClient client;

Button2 btn1(_D36_PIN);
Relay r1(_D38_PIN);
Relay r2(_D40_PIN);
Tstat tstat(r2);

SHTSensor sht(SHT_0x45);
float cTemp;

TaskHandle_t xHandle;

void setup() {
  Serial.begin(115200);
  // while (!Serial) ;
  Wire.begin(_SDA_PIN, _SCL_PIN);
  xTaskCreatePinnedToCore(vTaskCode, "vTaskCode", 2048, NULL,1, &xHandle, /* core */ 0);

  if (!LittleFS.begin()) {
    Serial.println(F("Failed to mount file system"));
  }
  loadConfigFile(configFilePath, config);

  EPD.setup();
  r1.setup();
  r2.setup();
  digitalWrite(btn1.getAttachPin(), LOW);
  btn1.setPressedHandler([](Button2& btn) { r1.toggle(); });

  if (sht.init()) {
    sht.setAccuracy(SHTSensor::SHT_ACCURACY_HIGH);
    tstat.setup(TstatMode::HEAT,
                /* setpoint */   25.5,
                /* hysteresis */ 1,
                /* lowerLimit */ 20,
                /* upperLimit */ 28,
                /* interval */   timeoutLength*10);
  } else {
    Serial.println(F("SHT initialization failed"));
  }

  char buf[64] = "Waiting for WiFi";
  EPD.getFonts()->setFont(u8g2_font_helvB10_tf);
  EPD.partialUpdate(buf);
  PersWiFi.begin();

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
  PersWiFi.handleWiFi();
  Alarm.delay(1000);

  if (sht.readSample()) {
    cTemp = sht.getTemperature();
    tstat.handle(cTemp);
    Serial.println(cTemp);
  }

  char buf[16];
  snprintf_P(buf,
             sizeof(buf),
             PSTR("%.2f°"),
             cTemp);
  EPD.getFonts()->setFont(u8g2_font_logisoso46_tf);
  EPD.partialUpdate(buf);

  ThingSpeak.setField(1, cTemp);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
}

void vTaskCode(void* pvParameter) {
  for(;;) {
    btn1.loop();
  }
}
