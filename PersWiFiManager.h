#ifndef _PERSWIFIMANAGER_H_
#define _PERSWIFIMANAGER_H_

#include <Arduino.h>
#include <elapsedMillis.h>
#include <WiFi.h>

typedef enum {
  PWM_WIFI_AWAIT,
  PWM_WIFI_CONNECTED,
  PWM_SMARTCONFIG_AWAIT
} PWMEvent_t;

typedef void (*WiFiOnReadyHandler)(PWMEvent_t event);

class PersWiFiManager {
  const unsigned long _timeoutLength = 60000;
  WiFiOnReadyHandler _onReadyHandler = NULL;
  void _awaitWiFi();
  void _awaitSmartConfig();

  public:
    void attemptConnection();
    void handleWiFi();
    void onReady(WiFiOnReadyHandler fn);
};

#endif
