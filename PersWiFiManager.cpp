#include "PersWiFiManager.h"

void PersWiFiManager::_awaitWiFi() {
  WiFi.begin();
  Serial.println(F("Waiting for WiFi"));
  if (_onReadyHandler) _onReadyHandler(PWMEvent_t::PWM_WIFI_AWAIT);
  elapsedMillis timeElapsed;
  while (WiFi.status() != WL_CONNECTED && timeElapsed < _timeoutLength) {
    delay(500);
    Serial.print(F("."));
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(F("WiFi Connected."));
    if (_onReadyHandler) _onReadyHandler(PWMEvent_t::PWM_WIFI_CONNECTED);
    Serial.print(F("IP Address: "));
    Serial.println(WiFi.localIP());
  }
}

void PersWiFiManager::_awaitSmartConfig() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.beginSmartConfig();
    Serial.println(F("Waiting for SmartConfig."));
    if (_onReadyHandler) _onReadyHandler(PWMEvent_t::PWM_SMARTCONFIG_AWAIT);
    while (!WiFi.smartConfigDone()) {
      delay(500);
      Serial.print(F("."));
    }
    Serial.println(F(""));
    Serial.println(F("SmartConfig received."));
    _awaitWiFi();
  }
}

void PersWiFiManager::attemptConnection() {
  WiFi.mode(WIFI_AP_STA);
  _awaitWiFi();
  _awaitSmartConfig();
}

void PersWiFiManager::handleWiFi() {}

void PersWiFiManager::onReady(WiFiOnReadyHandler fn) {
  _onReadyHandler = fn;
}