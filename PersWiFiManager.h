#ifndef _PERSWIFIMANAGER_H_
#define _PERSWIFIMANAGER_H_

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <elapsedMillis.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "Config.h"
#include "ESP32Helper.h"

class PersWiFiManager {
  const char* _poolServerName = "br.pool.ntp.org";
  const long  _timeOffset = -3*3600;
  WiFiUDP     _ntpUDP;
  NTPClient   _timeClient;
  void        _begin();
  void        _initOTA();
  void        _initNTP();

  public:
    PersWiFiManager():
                    _timeClient(_ntpUDP, _poolServerName, _timeOffset)
                    {};
    void begin();
    void handleWiFi();
};

extern PersWiFiManager PersWiFi;

#endif
