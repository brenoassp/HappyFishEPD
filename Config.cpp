#include "Config.h"

const char* configFilePath = "/config.txt";
Config config;

void ServerConfig::load(JsonObjectConst obj) {
  strncpy_P(hostname,
            obj["hostname"] | "myesp32",
            sizeof(hostname));
  strncpy_P(user,
            obj["user"] | "admin",
            sizeof(user));
  strncpy_P(pass,
            obj["pass"] | "admin",
            sizeof(pass));
}

void ServerConfig::save(JsonObject obj) const {
  obj["hostname"] = hostname;
  obj["user"] = user;
  obj["pass"] = pass;
}

void AlarmsConfig::load(JsonObjectConst obj) {
  on = obj["on"];
  off = obj["off"];
}

void AlarmsConfig::save(JsonObject obj) const {
  obj["on"] = on;
  obj["off"] = off;
}

void Config::load(JsonObjectConst obj) {
  server.load(obj["server"]);
  alarms.load(obj["alarms"]);
}

void Config::save(JsonObject obj) const {
  server.save(obj.createNestedObject("server"));
  alarms.save(obj.createNestedObject("alarms"));
}

bool deserializeConfig(Stream& src, Config& config) {
  const size_t capacity = 1024;
  StaticJsonDocument<capacity> doc;
  DeserializationError error = deserializeJson(doc, src);
  if (error) return false;
  config.load(doc.as<JsonObject>());
  return true;
}

bool serializeConfig(const Config& config, Print& dest) {
  const size_t capacity = 1024;
  StaticJsonDocument<capacity> doc;
  JsonObject root = doc.to<JsonObject>();
  config.save(root);
  return serializeJson(doc, dest) > 0;
}

bool loadConfigFile(const char* filename, Config& config) {
  File file = LittleFS.open(filename, "r");
  if (!file) {
    Serial.println(F("Failed to open config file"));
    return false;
  }
  bool success = deserializeConfig(file, config);
  file.close();
  if (!success) {
    Serial.println(F("Failed to deserialize config file"));
    return false;
  }
  return true;
}

bool saveConfigFile(const char* filename, const Config& config) {
  File file = LittleFS.open(filename, "w");
  if (!file) {
    Serial.println(F("Failed to create config file"));
    return false;
  }
  bool success = serializeConfig(config, file);
  file.close();
  if (!success) {
    Serial.println(F("Failed to serialize config file"));
    return false;
  }
  return true;
}
