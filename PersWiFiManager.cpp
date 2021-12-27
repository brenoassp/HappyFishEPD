#include "PersWiFiManager.h"

PersWiFiManager PersWiFi;
AsyncWebServer WS(80);
AsyncEventSource ES("/events");

void PersWiFiManager::_begin() {
  WiFi.setHostname(config.server.host);
  WiFi.begin();
}

void PersWiFiManager::_initOTA() {
  ArduinoOTA.setHostname((const char *)config.server.host);
  ArduinoOTA.setPassword((const char *)config.server.pass);
  ArduinoOTA.begin();
}

void PersWiFiManager::_initNTP() {
  _timeClient.begin();
  if (_timeClient.forceUpdate()) {
    RTC.setTime(_timeClient.getEpochTime());
  }
}

void PersWiFiManager::_initWS() {
  WS.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    if(!request->authenticate(
      (const char *)config.server.user,
      (const char *)config.server.pass
    ))
      return request->requestAuthentication();
    request->send(LittleFS, "/www/index.html");
  });
  WS.onNotFound([](AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
  });
  WS.serveStatic("/", LittleFS, "/www/")
  .setAuthentication(
    (const char *)config.server.user,
    (const char *)config.server.pass
  );
  WS.begin();

  ES.onConnect([](AsyncEventSourceClient *client) {
    if (client->lastId()) {
      Serial.printf("Client reconnected! Last message ID that it gat is: %u\n", client->lastId());
    }
    client->send("hello!", NULL, millis(), 1000);
  });
  ES.setAuthentication(
    (const char *)config.server.user,
    (const char *)config.server.pass
  );
  WS.addHandler(&ES);
}

void PersWiFiManager::begin() {
  WiFi.mode(WIFI_AP_STA);
  _begin();

  Serial.println(F("Waiting for WiFi."));
  elapsedMillis timeElapsed;
  while (WiFi.status() != WL_CONNECTED && timeElapsed < timeoutLength) {
    delay(500);
    Serial.print(F("."));
  }
  timeElapsed = 0;

  if (WiFi.status() != WL_CONNECTED) {
    WiFi.beginSmartConfig();
    Serial.println(F("Waiting for SmartConfig."));
    while (!WiFi.smartConfigDone()) {
      delay(500);
      Serial.print(F("."));
    }
    Serial.println(F(""));
    Serial.println(F("SmartConfig received."));

    Serial.println(F("Waiting for WiFi."));
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(F("."));
    }
  }

  Serial.println(F("WiFi Connected."));
  Serial.print(F("IP Address: "));
  Serial.println(WiFi.localIP());

  _initOTA();
  _initNTP();
  _initWS();
}

void PersWiFiManager::handleWiFi() {
  ArduinoOTA.handle();
  if (_timeClient.update()) {
    RTC.setTime(_timeClient.getEpochTime());
  }
}
