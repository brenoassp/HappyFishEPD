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
#include "ESP32Helper.h"

class PersWiFiManager {
  const char*       _poolServerName = "br.pool.ntp.org";
  const long        _timeOffset = -3*3600;
  Config*           _config;
  elapsedMillis     _timeElapsed;
  WiFiUDP           _ntpUDP;
  NTPClient         _timeClient;
  ESP32Time*        _rtc;
  void              _begin();
  void              _initOTA();
  void              _initNTP();

  public:
    PersWiFiManager(Config* config, ESP32Time* rtc):
                    _config(config),
                    _rtc(rtc),
                    _timeClient(_ntpUDP, _poolServerName, _timeOffset) {};
    void begin();
    void handleWiFi();
};

#endif
