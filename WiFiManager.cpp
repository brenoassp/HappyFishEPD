#include "WiFiManager.h"

WiFiManagerClass WiFiManager;
AsyncWebServer server(80);
AsyncEventSource events("/events");
WiFiClient espClient;
PubSubClient mqttClient(espClient);

void WiFiManagerClass::_begin() {
  WiFi.setHostname(config.server.host);
  WiFi.begin();
}

void WiFiManagerClass::_initOTA() {
  ArduinoOTA.setHostname((const char *)config.server.host);
  ArduinoOTA.setPassword((const char *)config.server.pass);
  ArduinoOTA.begin();
}

void WiFiManagerClass::_initNTP() {
  _timeClient.begin();
  if (_timeClient.forceUpdate()) {
    RTC.setTime(_timeClient.getEpochTime());
  }
}

void WiFiManagerClass::_initWebServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    if(!request->authenticate(
      (const char *)config.server.user,
      (const char *)config.server.pass
    ))
      return request->requestAuthentication();
    request->send(LittleFS, "/www/index.html");
  });
  server.onNotFound([](AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
  });
  server.serveStatic("/", LittleFS, "/www/")
  .setAuthentication(
    (const char *)config.server.user,
    (const char *)config.server.pass
  );
  server.begin();

  events.onConnect([](AsyncEventSourceClient *client) {
    if (client->lastId()) {
      Serial.printf("Client reconnected! Last message ID that it gat is: %u\n", client->lastId());
    }
    client->send("hello!", NULL, millis(), 1000);
  });
  events.setAuthentication(
    (const char *)config.server.user,
    (const char *)config.server.pass
  );
  server.addHandler(&events);
}

void WiFiManagerClass::_initMQTTClient() {
  mqttClient.setServer(
    (const char *)config.mqtt.server,
    config.mqtt.port
  );
  mqttClient.setBufferSize(2048);
}

void WiFiManagerClass::begin() {
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
  _initWebServer();
  _initMQTTClient();
}

void WiFiManagerClass::handleWiFi() {
  ArduinoOTA.handle();

  if (_timeClient.update()) {
    RTC.setTime(_timeClient.getEpochTime());
  }

  if (!mqttClient.connected()) {
    mqttClient.connect(
      (const char *)config.mqtt.clientId,
      (const char *)config.mqtt.user,
      (const char *)config.mqtt.pass
    );
    mqttClient.subscribe("channels/1606204/subscribe");
  }
  mqttClient.loop();
}
