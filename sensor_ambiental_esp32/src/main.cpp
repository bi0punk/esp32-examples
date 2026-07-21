#include <WiFiManager.h>
#include "config.h"
#include "SensorManager.h"
#include "DataPublisher.h"

SensorConfig config;
SensorManager *sensor = nullptr;
DataPublisher *publisher = nullptr;
unsigned long lastSample = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFiManager wm;
  wm.setConfigPortalTimeout(180);
  bool connected = wm.autoConnect("ESP32-Sensor");

  if (connected) {
    Serial.println("[WiFi] Conectado: " + WiFi.localIP().toString());
  } else {
    Serial.println("[WiFi] No conectado, modo sin red");
  }

  loadConfig(config);

  sensor = new SensorManager(config);
  publisher = new DataPublisher(config);

  sensor->begin();
  publisher->beginDisplay();

  Serial.println("[sensor_ambiental] Listo");
}

void loop() {
  unsigned long now = millis();
  if (now - lastSample < (unsigned long)config.sampleIntervalSec * 1000) return;
  lastSample = now;

  SensorReading r = sensor->read();
  publisher->publish(r, now);
}
