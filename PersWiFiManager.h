#ifndef _PERSWIFIMANAGER_H_
#define _PERSWIFIMANAGER_H_

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <elapsedMillis.h>
#include <WiFi.h>
#include "Config.h"

typedef enum {
  PWM_WIFI_AWAIT,
  PWM_WIFI_CONNECTED,
  PWM_SMARTCONFIG_AWAIT
} pwm_event_t;

typedef void (*OnWiFiBegin)(pwm_event_t event);

class PersWiFiManager {
  const unsigned long _timeoutLength = 60000;
  Config *_config;
  void _initOTA();

  public:
    PersWiFiManager(Config *config): _config(config) {};
    void begin(OnWiFiBegin handleWifiOnBegin);
    void handleWiFi();
};

#endif
