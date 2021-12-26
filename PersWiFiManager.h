#ifndef _PERSWIFIMANAGER_H_
#define _PERSWIFIMANAGER_H_

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <AsyncTCP.h>
#include <elapsedMillis.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "ESP32Helper.h"
#include "Config.h"

class PersWiFiManager {
  const char* _poolServerName = "br.pool.ntp.org";
  const long  _timeOffset = -3*3600;
  WiFiUDP     _ntpUDP;
  NTPClient   _timeClient;
  void        _begin();
  void        _initOTA();
  void        _initNTP();
  void        _initWS();

  public:
    PersWiFiManager():
                    _timeClient(_ntpUDP, _poolServerName, _timeOffset)
                    {};
    void begin();
    void handleWiFi();
};

extern PersWiFiManager PersWiFi;
extern AsyncWebServer AWebServer;
extern AsyncEventSource AEventSource;

#endif
