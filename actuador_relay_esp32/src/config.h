#pragma once

#include <ArduinoJson.h>
#include <SPIFFS.h>

#define CONFIG_FILE "/config.json"

struct RelayConfig {
  int relayPin = 16;
  bool relayActiveHigh = true;
  bool relayEnabled = true;

  bool useUltrasonic = false;
  int trigPin = 17;
  int echoPin = 18;
  int triggerDistanceCm = 100;
  int triggerDurationMs = 3000;

  bool useAutoCycle = false;
  int cycleOnMs = 10000;
  int cycleOffMs = 10000;

  bool useWebServer = true;
  bool useSerial = true;
};

bool loadRelayConfig(RelayConfig &cfg) {
  if (!SPIFFS.begin(true)) return false;
  File f = SPIFFS.open(CONFIG_FILE, "r");
  if (!f) return false;

  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, f);
  f.close();
  if (err) return false;

  cfg.relayPin = doc["relayPin"] | 16;
  cfg.relayActiveHigh = doc["relayActiveHigh"] | true;
  cfg.relayEnabled = doc["relayEnabled"] | true;
  cfg.useUltrasonic = doc["useUltrasonic"] | false;
  cfg.trigPin = doc["trigPin"] | 17;
  cfg.echoPin = doc["echoPin"] | 18;
  cfg.triggerDistanceCm = doc["triggerDistanceCm"] | 100;
  cfg.triggerDurationMs = doc["triggerDurationMs"] | 3000;
  cfg.useAutoCycle = doc["useAutoCycle"] | false;
  cfg.cycleOnMs = doc["cycleOnMs"] | 10000;
  cfg.cycleOffMs = doc["cycleOffMs"] | 10000;
  cfg.useWebServer = doc["useWebServer"] | true;
  cfg.useSerial = doc["useSerial"] | true;

  return true;
}
