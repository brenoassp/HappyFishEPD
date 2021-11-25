#include "PersWiFiManager.h"

void PersWiFiManager::loadPrefs(Preferences &prefs) {
  prefs.begin("perswm", false);
  strncpy_P(_hostname,
            prefs.getString("hostname", "myesp32s2").c_str(),
            sizeof(_hostname));
  prefs.end();
}

void PersWiFiManager::attemptConnection() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.setHostname(_hostname);
  WiFi.begin();

  Serial.println(F("Waiting for WiFi."));
  handleOnReady(PWM_WIFI_AWAIT);
  elapsedMillis timeElapsed;
  while (WiFi.status() != WL_CONNECTED && timeElapsed < _timeoutLength) {
    delay(500);
    Serial.print(F("."));
  }

  if (WiFi.status() != WL_CONNECTED) {
    WiFi.beginSmartConfig();
    Serial.println(F("Waiting for SmartConfig."));
    handleOnReady(PWM_SMARTCONFIG_AWAIT);
    while (!WiFi.smartConfigDone()) {
      delay(500);
      Serial.print(F("."));
    }
    Serial.println(F(""));
    Serial.println(F("SmartConfig received."));

    Serial.println(F("Waiting for WiFi."));
    handleOnReady(PWM_WIFI_AWAIT);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(F("."));
    }
  }

  Serial.println(F("WiFi Connected."));
  handleOnReady(PWM_WIFI_CONNECTED);
  Serial.print(F("IP Address: "));
  Serial.println(WiFi.localIP());
}

void PersWiFiManager::handleWiFi() {}

void PersWiFiManager::onReady(WiFiOnReadyHandler fn) {
  _onReadyHandler = fn;
}

void PersWiFiManager::handleOnReady(pwm_event_t event) {
  if (_onReadyHandler) _onReadyHandler(event);
}