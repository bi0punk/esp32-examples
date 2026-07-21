#pragma once

#include <ArduinoJson.h>
#include <SPIFFS.h>

#define CONFIG_FILE "/config.json"

struct SueloConfig {
  int sensorPin = A0;
  float dryValue = 1000.0f;
  float wetValue = 400.0f;
  int sampleIntervalSec = 60;
  int relayPins[4] = {-1, -1, -1, -1};
  int relayCount = 0;

  bool useSerial = true;
  bool useHTTP = false;
  String httpUrl = "http://192.168.1.100:8000/update_data";
  bool useWebServer = false;
  bool useNTP = false;
  int utcOffset = -3;
};

bool loadSueloConfig(SueloConfig &cfg) {
  if (!SPIFFS.begin(true)) return false;
  File f = SPIFFS.open(CONFIG_FILE, "r");
  if (!f) return false;

  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, f);
  f.close();
  if (err) return false;

  cfg.sensorPin = doc["sensorPin"] | A0;
  cfg.dryValue = doc["dryValue"] | 1000.0f;
  cfg.wetValue = doc["wetValue"] | 400.0f;
  cfg.sampleIntervalSec = doc["sampleIntervalSec"] | 60;
  cfg.useSerial = doc["useSerial"] | true;
  cfg.useHTTP = doc["useHTTP"] | false;
  cfg.httpUrl = doc["httpUrl"] | "http://192.168.1.100:8000/update_data";
  cfg.useWebServer = doc["useWebServer"] | false;
  cfg.useNTP = doc["useNTP"] | false;
  cfg.utcOffset = doc["utcOffset"] | -3;

  if (doc["relayPins"].is<JsonArray>()) {
    cfg.relayCount = 0;
    for (JsonVariant v : doc["relayPins"].as<JsonArray>()) {
      if (cfg.relayCount < 4) cfg.relayPins[cfg.relayCount++] = v;
    }
  }
  return true;
}
