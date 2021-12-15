#include "PersWiFiManager.h"

void PersWiFiManager::_begin() {
  WiFi.setHostname(config.server.hostname);
  WiFi.begin();
}

void PersWiFiManager::_initOTA() {
  ArduinoOTA.setHostname((const char *)config.server.hostname);
  ArduinoOTA.setPassword((const char *)config.server.pass);
  ArduinoOTA.begin();
}

void PersWiFiManager::_initNTP() {
  _timeClient.begin();
  _timeClient.forceUpdate();
}

void PersWiFiManager::begin() {
  WiFi.mode(WIFI_AP_STA);
  _begin();

  Serial.println(F("Waiting for WiFi."));
  while (WiFi.status() != WL_CONNECTED && _timeElapsed < timeoutLength) {
    delay(500);
    Serial.print(F("."));
  }
  _timeElapsed = 0;

  if (WiFi.status() != WL_CONNECTED) {
    WiFi.beginSmartConfig();
    Serial.println(F("Waiting for SmartConfig."));
    while (!WiFi.smartConfigDone()) {
      delay(500);
      Serial.print(F("."));
    }
    Serial.println(F(""));
    Serial.println(F("SmartConfig received."));

    Serial.println(F("Waiting for WiFi."));
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(F("."));
    }
  }

  Serial.println(F("WiFi Connected."));
  Serial.print(F("IP Address: "));
  Serial.println(WiFi.localIP());

  _initOTA();
  _initNTP();
}

void PersWiFiManager::handleWiFi() {
  ArduinoOTA.handle();

  if (_timeClient.update()) {
    rtc.setTime(_timeClient.getEpochTime());
  }

  /*if (WiFi.status() != WL_CONNECTED && _timeElapsed >= timeoutLength) {
    Serial.println(F("Reconnecting to WiFi..."));
    WiFi.disconnect();
    WiFi.reconnect();
    _timeElapsed = 0;
  }*/
}
