#ifndef _WIFIMANAGER_H_
#define _WIFIMANAGER_H_

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <AsyncTCP.h>
#include <elapsedMillis.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "S2MiniHelper.h"
#include "Config.h"

extern AsyncWebServer server;
extern AsyncEventSource events;

class WiFiManagerClass {
  const char *_poolServerName = "br.pool.ntp.org";
  const long _timeOffset = -3*3600;
  WiFiUDP _ntpUDP;
  NTPClient _timeClient;
  void _begin();
  void _initOTA();
  void _initNTP();
  void _initWebServer();

  public:
    WiFiManagerClass():
                      _timeClient(_ntpUDP, _poolServerName, _timeOffset)
                      {};
    void begin();
    void handleWiFi();
};

extern WiFiManagerClass WiFiManager;

#endif
