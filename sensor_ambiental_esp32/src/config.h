#pragma once

#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>

#define CONFIG_FILE "/config.json"

struct SensorConfig {
  int dhtPin = 4;
  int dhtType = 22;        // 11 o 22
  bool useDHT = false;

  bool useSHT31 = false;
  bool useSHTC3 = false;

  bool useMQ135 = false;
  int mq135Pin = 34;

  bool useOLED = false;
  int oledSDA = 21;
  int oledSCL = 22;

  int sampleIntervalSec = 5;

  // HTTP endpoint
  bool useHTTP = false;
  String httpUrl = "http://192.168.1.100:8000/sensor";

  // Serial output
  bool useSerial = true;
};

bool loadConfig(SensorConfig &cfg) {
  if (!SPIFFS.begin(true)) return false;

  File f = SPIFFS.open(CONFIG_FILE, "r");
  if (!f) return false;

  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, f);
  f.close();
  if (err) return false;

  cfg.dhtPin       = doc["dhtPin"] | 4;
  cfg.dhtType      = doc["dhtType"] | 22;
  cfg.useDHT       = doc["useDHT"] | false;
  cfg.useSHT31     = doc["useSHT31"] | false;
  cfg.useSHTC3     = doc["useSHTC3"] | false;
  cfg.useMQ135     = doc["useMQ135"] | false;
  cfg.mq135Pin     = doc["mq135Pin"] | 34;
  cfg.useOLED      = doc["useOLED"] | false;
  cfg.oledSDA      = doc["oledSDA"] | 21;
  cfg.oledSCL      = doc["oledSCL"] | 22;
  cfg.sampleIntervalSec = doc["sampleIntervalSec"] | 5;
  cfg.useHTTP      = doc["useHTTP"] | false;
  cfg.httpUrl      = doc["httpUrl"] | "http://192.168.1.100:8000/sensor";
  cfg.useSerial    = doc["useSerial"] | true;

  return true;
}
