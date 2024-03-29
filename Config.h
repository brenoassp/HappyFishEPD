#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

extern const char *configFilePath;

struct ServerConfig {
  char host[32];
  char user[32];
  char pass[32];
  void load(JsonObjectConst);
  void save(JsonObject) const;
};

struct AlarmsConfig {
  int32_t on;
  int32_t off;
  void load(JsonObjectConst);
  void save(JsonObject) const;
};

struct MQTTConfig {
  char server[64];
  int32_t port;
  char user[64];
  char pass[64];
  char clientId[64];
  void load(JsonObjectConst);
  void save(JsonObject) const;
};

struct Config {
  ServerConfig server;
  AlarmsConfig alarms;
  MQTTConfig mqtt;
  void load(JsonObjectConst);
  void save(JsonObject) const;
};

bool deserializeConfig(Stream &src, Config &config);
bool serializeConfig(const Config &config, Print &dest);
bool loadConfigFile(const char *filename, Config &config);
bool saveConfigFile(const char *filename, const Config &config);

extern Config config;

#endif