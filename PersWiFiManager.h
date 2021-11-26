#ifndef _PERSWIFIMANAGER_H_
#define _PERSWIFIMANAGER_H_

#include <Arduino.h>
#include <elapsedMillis.h>
#include <WiFi.h>

typedef enum {
  PWM_WIFI_AWAIT,
  PWM_WIFI_CONNECTED,
  PWM_SMARTCONFIG_AWAIT
} pwm_event_t;

typedef void (*OnWiFiBegin)(pwm_event_t event);

class PersWiFiManager {
  const unsigned long _timeoutLength = 60000;
  char _hostname[32] = "esp32s2";

  public:
    void begin(OnWiFiBegin handleWifiOnBegin);
    void handleWiFi();
};

#endif
