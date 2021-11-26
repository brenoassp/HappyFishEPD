#include "PersWiFiManager.h"

void PersWiFiManager::begin(OnWiFiBegin handleWiFiBegin) {
  WiFi.mode(WIFI_AP_STA);
  WiFi.setHostname(_hostname);
  WiFi.begin();

  Serial.println(F("Waiting for WiFi."));
  if (handleWiFiBegin) handleWiFiBegin(PWM_WIFI_AWAIT);
  elapsedMillis timeElapsed;
  while (WiFi.status() != WL_CONNECTED && timeElapsed < _timeoutLength) {
    delay(500);
    Serial.print(F("."));
  }

  if (WiFi.status() != WL_CONNECTED) {
    WiFi.beginSmartConfig();
    Serial.println(F("Waiting for SmartConfig."));
    if (handleWiFiBegin) handleWiFiBegin(PWM_SMARTCONFIG_AWAIT);
    while (!WiFi.smartConfigDone()) {
      delay(500);
      Serial.print(F("."));
    }
    Serial.println(F(""));
    Serial.println(F("SmartConfig received."));

    Serial.println(F("Waiting for WiFi."));
    if (handleWiFiBegin) handleWiFiBegin(PWM_WIFI_AWAIT);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(F("."));
    }
  }

  Serial.println(F("WiFi Connected."));
  if (handleWiFiBegin) handleWiFiBegin(PWM_WIFI_CONNECTED);
  Serial.print(F("IP Address: "));
  Serial.println(WiFi.localIP());
}

void PersWiFiManager::handleWiFi() {}
