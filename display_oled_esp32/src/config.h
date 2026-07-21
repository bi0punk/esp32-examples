#pragma once

#include <ArduinoJson.h>
#include <SPIFFS.h>

#define CONFIG_FILE "/config.json"

enum DisplayMode {
  MODE_BITMAP,
  MODE_EFFECTS,
  MODE_SCROLL,
  MODE_SENSOR,
  MODE_SISMO,
  MODE_TFT_BMP,
  MODE_SEVENSEG
};

struct DisplayConfig {
  int displayType = 0; // 0=OLED, 1=TFT, 2=7seg
  int oledSDA = 21;
  int oledSCL = 22;
  int oledAddr = 0x3C;

  DisplayMode mode = MODE_EFFECTS;
  int scrollSpeed = 50;
  String scrollText = "ESP32 Display Demo";
  String bitmapFile = "/picture.bmp";
  int sismoIntervalMin = 5;
  String sismoApiUrl = "http://192.168.1.100:8000/api/sismo/latest/";

  // 7-segment pins {A,B,C,D,E,F,G,DP,D1,D2,D3,D4}
  int segPins[12] = {2,3,4,5,6,7,8,9,10,11,12,13};
  int segDigits = 1;
  bool segCommonCathode = true;

  int dhtPin = 4;
  int dhtType = 22;
  bool useSensor = false;
  bool useWiFi = true;
};

bool loadDisplayConfig(DisplayConfig &cfg) {
  if (!SPIFFS.begin(true)) return false;
  File f = SPIFFS.open(CONFIG_FILE, "r");
  if (!f) return false;

  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, f);
  f.close();
  if (err) return false;

  cfg.displayType = doc["displayType"] | 0;
  cfg.oledSDA = doc["oledSDA"] | 21;
  cfg.oledSCL = doc["oledSCL"] | 22;
  cfg.mode = (DisplayMode)(doc["mode"] | 1);
  cfg.scrollSpeed = doc["scrollSpeed"] | 50;
  cfg.scrollText = doc["scrollText"] | "ESP32 Display Demo";
  cfg.useWiFi = doc["useWiFi"] | true;
  cfg.sismoIntervalMin = doc["sismoIntervalMin"] | 5;
  cfg.sismoApiUrl = doc["sismoApiUrl"] | "http://192.168.1.100:8000/api/sismo/latest/";
  cfg.useSensor = doc["useSensor"] | false;
  cfg.dhtPin = doc["dhtPin"] | 4;
  cfg.dhtType = doc["dhtType"] | 22;
  cfg.segDigits = doc["segDigits"] | 1;
  cfg.segCommonCathode = doc["segCommonCathode"] | true;

  if (doc["segPins"].is<JsonArray>()) {
    int i = 0;
    for (JsonVariant v : doc["segPins"].as<JsonArray>()) {
      if (i < 12) cfg.segPins[i++] = v;
    }
  }
  return true;
}
