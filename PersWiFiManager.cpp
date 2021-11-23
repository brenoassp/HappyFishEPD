#include "PersWiFiManager.h"

void PersWiFiManager::attemptConnection() {
  WiFi.mode(WIFI_AP_STA);

  WiFi.begin();
  elapsedMillis timeElapsed;
  Serial.println(F("Waiting for WiFi"));
  while (WiFi.status() != WL_CONNECTED && timeElapsed < _timeoutLength) {
    delay(500);
    Serial.print(F("."));
  }
  timeElapsed = 0;

  if (WiFi.status() != WL_CONNECTED) {
    WiFi.beginSmartConfig();
    Serial.println(F("Waiting for SmartConfig."));
    while (!WiFi.smartConfigDone()) {
      delay(500);
      Serial.print(F("."));
    }
    Serial.println(F(""));
    Serial.println(F("SmartConfig received."));

    Serial.println(F("Waiting for WiFi"));
    while (WiFi.status() != WL_CONNECTED && timeElapsed < _timeoutLength) {
      delay(500);
      Serial.print(F("."));
    }
  }

  Serial.println(F("WiFi Connected."));

  Serial.print(F("IP Address: "));
  Serial.println(WiFi.localIP());
}

void PersWiFiManager::handleWiFi() {}
