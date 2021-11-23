#ifndef _PERSWIFIMANAGER_H_
#define _PERSWIFIMANAGER_H_

#include <Arduino.h>
#include <elapsedMillis.h>
#include <WiFi.h>

class PersWiFiManager {
  const unsigned long _timeoutLength = 60000;

  public:
    void attemptConnection();
    void handleWiFi();
};

#endif
