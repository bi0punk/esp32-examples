#pragma once

#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_SSD1306.h>
#include "config.h"
#include "SensorManager.h"

class DataPublisher {
public:
  DataPublisher(const SensorConfig &cfg) : cfg_(cfg) {}

  bool beginDisplay() {
    if (!cfg_.useOLED) return false;
    Wire.begin(cfg_.oledSDA, cfg_.oledSCL);
    display_ = new Adafruit_SSD1306(128, 64, &Wire, -1);
    if (!display_->begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      delete display_;
      display_ = nullptr;
      return false;
    }
    display_->clearDisplay();
    display_->display();
    return true;
  }

  void publish(const SensorReading &r, unsigned long uptime) {
    if (cfg_.useSerial) printSerial(r);
    if (cfg_.useOLED && display_) printOLED(r);
    if (cfg_.useHTTP) sendHTTP(r);
  }

private:
  const SensorConfig &cfg_;
  Adafruit_SSD1306 *display_ = nullptr;

  void printSerial(const SensorReading &r) {
    Serial.print("[sensor_ambiental] ");
    if (!isnan(r.temperature)) {
      Serial.printf("Temp: %.2f C  ", r.temperature);
    }
    if (!isnan(r.humidity)) {
      Serial.printf("Hum: %.2f %%  ", r.humidity);
    }
    if (!isnan(r.gasVoltage)) {
      Serial.printf("Gas: %.2f V (raw: %.0f)", r.gasVoltage, r.gasRaw);
    }
    Serial.println();
  }

  void printOLED(const SensorReading &r) {
    display_->clearDisplay();
    display_->setTextSize(1);
    display_->setTextColor(SSD1306_WHITE);
    display_->setCursor(0, 0);

    if (!isnan(r.temperature)) {
      display_->print("Temp: ");
      display_->print(r.temperature, 1);
      display_->println(" C");
    }
    if (!isnan(r.humidity)) {
      display_->print("Hum:  ");
      display_->print(r.humidity, 1);
      display_->println(" %");
    }
    if (!isnan(r.gasVoltage)) {
      display_->print("Gas:  ");
      display_->print(r.gasVoltage, 2);
      display_->println(" V");
    }
    display_->display();
  }

  void sendHTTP(const SensorReading &r) {
    if (WiFi.status() != WL_CONNECTED) return;

    JsonDocument doc;
    if (!isnan(r.temperature)) doc["temperature"] = r.temperature;
    if (!isnan(r.humidity)) doc["humidity"] = r.humidity;
    if (!isnan(r.gasRaw)) doc["gas_raw"] = r.gasRaw;
    if (!isnan(r.gasVoltage)) doc["gas_voltage"] = r.gasVoltage;

    String body;
    serializeJson(doc, body);

    HTTPClient http;
    http.begin(cfg_.httpUrl);
    http.addHeader("Content-Type", "application/json");
    int code = http.POST(body);
    if (code > 0) {
      Serial.printf("[HTTP] POST %d\n", code);
    } else {
      Serial.printf("[HTTP] Error: %s\n", http.errorToString(code).c_str());
    }
    http.end();
  }
};
