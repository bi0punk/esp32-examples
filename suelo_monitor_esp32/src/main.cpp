#include <WiFi.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "config.h"

SueloConfig cfg;
WiFiUDP ntpUDP;
NTPClient *timeClient = nullptr;
WiFiServer *webServer = nullptr;
unsigned long lastSample = 0;

float readMoisture() {
  int raw = analogRead(cfg.sensorPin);
  float pct = map(raw, cfg.dryValue, cfg.wetValue, 0, 100);
  pct = constrain(pct, 0.0f, 100.0f);
  Serial.printf("[suelo] raw=%d pct=%.1f%%\n", raw, pct);
  return pct;
}

void sendHTTP(float pct, int raw) {
  if (WiFi.status() != WL_CONNECTED) return;
  HTTPClient http;
  JsonDocument doc;
  doc["humedad"] = pct;
  doc["raw"] = raw;
  String body;
  serializeJson(doc, body);
  http.begin(cfg.httpUrl);
  http.addHeader("Content-Type", "application/json");
  int code = http.POST(body);
  Serial.printf("[HTTP] POST %d\n", code);
  http.end();
}

void handleWebClient() {
  if (!webServer) return;
  WiFiClient c = webServer->available();
  if (!c) return;
  String req = c.readStringUntil('\r');
  c.flush();

  int raw = analogRead(cfg.sensorPin);
  float pct = readMoisture();

  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'><meta name='viewport' content='width=device-width'>";
  html += "<style>body{font-family:sans-serif;padding:2em;text-align:center}</style>";
  html += "</head><body>";
  html += "<h1>Monitor de Suelo</h1>";
  html += "<p>Humedad: <strong>" + String(pct, 1) + "%</strong></p>";
  html += "<p>Valor crudo: " + String(raw) + "</p>";
  if (timeClient) {
    timeClient->update();
    html += "<p>Hora: " + timeClient->getFormattedTime() + "</p>";
  }
  html += "</body></html>";

  c.println("HTTP/1.1 200 OK");
  c.println("Content-Type: text/html");
  c.println("Connection: close");
  c.println();
  c.println(html);
  c.stop();
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFiManager wm;
  wm.setConfigPortalTimeout(180);
  wm.autoConnect("ESP32-Suelo");

  loadSueloConfig(cfg);

  if (cfg.useNTP) {
    timeClient = new NTPClient(ntpUDP, "cl.pool.ntp.org", cfg.utcOffset * 3600);
    timeClient->begin();
  }

  if (cfg.useWebServer) {
    webServer = new WiFiServer(80);
    webServer->begin();
    Serial.println("[web] Servidor HTTP en puerto 80");
  }

  for (int i = 0; i < cfg.relayCount; i++) {
    if (cfg.relayPins[i] >= 0) {
      pinMode(cfg.relayPins[i], OUTPUT);
      digitalWrite(cfg.relayPins[i], HIGH);
    }
  }

  Serial.println("[suelo_monitor] Listo");
}

void loop() {
  if (webServer) handleWebClient();

  unsigned long now = millis();
  if (now - lastSample < (unsigned long)cfg.sampleIntervalSec * 1000) return;
  lastSample = now;

  int raw = analogRead(cfg.sensorPin);
  float pct = readMoisture();

  if (cfg.useSerial) {
    Serial.printf("[suelo] humedad=%.1f%% raw=%d\n", pct, raw);
  }
  if (cfg.useHTTP) sendHTTP(pct, raw);
}
