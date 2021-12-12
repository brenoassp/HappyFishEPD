#ifndef _PERSWIFIMANAGER_H_
#define _PERSWIFIMANAGER_H_

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <elapsedMillis.h>
#include <ESP32Time.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "Config.h"

typedef enum {
  PWM_WIFI_AWAIT,
  PWM_WIFI_CONNECTED,
  PWM_SMARTCONFIG_AWAIT
} pwm_event_t;

typedef void (*OnWiFiBegin)(pwm_event_t event);

class PersWiFiManager {
  const unsigned long _timeoutLength = 60000;
  const char *_poolServerName = "br.pool.ntp.org";
  const long _timeOffset = -3*3600;
  Config *_config;
  ESP32Time *_rtc;
  WiFiUDP _ntpUDP;
  NTPClient _timeClient;
  void _initOTA();
  void _initNTP();

  public:
    PersWiFiManager(Config *config, ESP32Time *rtc):
      _config(config),
      _rtc(rtc),
      _timeClient(_ntpUDP, _poolServerName, _timeOffset) {};
    void begin(OnWiFiBegin handleWifiOnBegin);
    void handleWiFi();
};

#endif
