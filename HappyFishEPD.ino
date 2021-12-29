#include <Button2.h>
#include <LittleFS.h>
#include <PubSubClient.h>
#include <SHTSensor.h>
#include <TimeAlarms.h>
#include <TimeLib.h>
#include <Wire.h>
#include "S2MiniHelper.h"
#include "Config.h"
#include "EPD.h"
#include "WiFiManager.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

Button2 btn1(_D36_PIN);
Relay r1(_D38_PIN);
Relay r2(_D40_PIN);
Tstat tstat(r2);

SHTSensor sht(SHT_0x44);
float cTemp;

unsigned long previousMillis = 0;
TaskHandle_t xHandle;

void setup() {
  Serial.begin(115200);
  // while (!Serial) ;
  Wire.begin(_SDA_PIN, _SCL_PIN);
  xTaskCreatePinnedToCore(vTaskCode, "vTaskCode", 2048, NULL, 1, &xHandle, /* core */ 0);

  if (!LittleFS.begin()) {
    Serial.println(F("Failed to mount file system"));
  }
  loadConfigFile(configFilePath, config);

  EPD.setup();
  r1.setup();
  r2.setup();
  digitalWrite(btn1.getAttachPin(), LOW);
  btn1.setPressedHandler([](Button2 &btn) { r1.toggle(); });

  if (sht.init()) {
    sht.setAccuracy(SHTSensor::SHT_ACCURACY_HIGH);
    tstat.setup(TstatMode::HEAT,
                /* setpoint */   25.5,
                /* hysteresis */ 1,
                /* lowerLimit */ 18,
                /* upperLimit */ 28,
                /* interval */   timeoutLength*10);
  } else {
    Serial.println(F("SHT initialization failed"));
  }

  char buf[64] = "Waiting for WiFi";
  u8g2Fonts.setFont(u8g2_font_helvB10_tf);
  EPD.partialUpdate(buf);
  WiFiManager.begin();

  setSyncProvider([]() { return RTC.getEpoch(); });
  Alarm.alarmRepeat(0,0,config.alarms.on, []() { r1.turnOn(); });
  Alarm.alarmRepeat(0,0,config.alarms.off, []() { r1.turnOff(); });

  mqttClient.setServer(
    (const char *)config.mqtt.server,
    config.mqtt.port
  );
  mqttClient.setBufferSize(2048);
}

void loop() {
  if (shouldReboot) {
    Serial.println(F("Rebooting..."));
    delay(100);
    ESP.restart();
  }
  WiFiManager.handleWiFi();

  if (sht.readSample()) {
    cTemp = sht.getTemperature();
    tstat.handle(cTemp);
  }

  char buf[16];
  snprintf_P(buf,
             sizeof(buf),
             PSTR("%.2f°"),
             cTemp);
  u8g2Fonts.setFont(u8g2_font_logisoso46_tf);
  EPD.partialUpdate(buf);

  if (!mqttClient.connected()) {
    mqttClient.connect(
      (const char *)config.mqtt.clientId,
      (const char *)config.mqtt.user,
      (const char *)config.mqtt.pass
    );
    mqttClient.subscribe("channels/1606204/subscribe");
  }
  mqttClient.loop();

  String s(mqttClient.state());
  events.send(s.c_str(), "message", millis());

  if (millis() - previousMillis > 2000) {
    previousMillis = millis();
    dtostrf(cTemp, 5, 2, buf);
    mqttClient.publish("channels/1606204/publish/fields/field1", buf);
  }

  // End of loop calls
  Alarm.delay(1000);
}

void vTaskCode(void *pvParameter) {
  for(;;) {
    btn1.loop();
  }
}
