#ifndef _PERSWIFIMANAGER_H_
#define _PERSWIFIMANAGER_H_

#include <Arduino.h>
#include <elapsedMillis.h>
#include <Preferences.h>
#include <WiFi.h>

typedef enum {
  PWM_WIFI_AWAIT,
  PWM_WIFI_CONNECTED,
  PWM_SMARTCONFIG_AWAIT
} pwm_event_t;

typedef void (*WiFiOnReadyHandler)(pwm_event_t event);

class PersWiFiManager {
  const unsigned long _timeoutLength = 60000;
  WiFiOnReadyHandler _onReadyHandler = NULL;
  char _hostname[32];

  public:
    void loadPrefs(Preferences &prefs);
    void attemptConnection();
    void handleWiFi();
    void onReady(WiFiOnReadyHandler fn);
    void handleOnReady(pwm_event_t event);
};

#endif
