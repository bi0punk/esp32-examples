#include <WiFi.h>
#include <WiFiManager.h>
#include "config.h"

RelayConfig cfg;
WiFiServer *server = nullptr;
bool relayState = false;
unsigned long lastTriggerTime = 0;
unsigned long cycleStartTime = 0;

void setRelay(bool on) {
  relayState = on;
  digitalWrite(cfg.relayPin, on == cfg.relayActiveHigh ? HIGH : LOW);
  if (cfg.useSerial) Serial.printf("[relay] %s\n", on ? "ON" : "OFF");
}

long measureDistance() {
  digitalWrite(cfg.trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(cfg.trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(cfg.trigPin, LOW);
  long duration = pulseIn(cfg.echoPin, HIGH, 30000);
  if (duration == 0) return 999;
  return duration * 0.034 / 2;
}

String htmlPage() {
  String h = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width'><style>";
  h += "body{font-family:sans-serif;padding:2em;text-align:center}";
  h += ".btn{padding:1em 3em;margin:1em;font-size:1.2em;border:none;border-radius:8px;cursor:pointer}";
  h += ".on{background:#4CAF50;color:#fff}.off{background:#f44336;color:#fff}";
  h += "</style></head><body>";
  h += "<h1>Control Relay</h1>";
  h += "<p>Estado: <strong>" + String(relayState ? "ON" : "OFF") + "</strong></p>";
  h += "<a href='/on' class='btn on'>ON</a>";
  h += "<a href='/off' class='btn off'>OFF</a>";
  if (cfg.useUltrasonic) {
    h += "<p>Distancia: " + String(measureDistance()) + " cm</p>";
  }
  h += "</body></html>";
  return h;
}

void handleClient() {
  if (!server) return;
  WiFiClient c = server->available();
  if (!c) return;
  String req = c.readStringUntil('\r');
  c.flush();

  if (req.indexOf("GET /on") >= 0) setRelay(true);
  else if (req.indexOf("GET /off") >= 0) setRelay(false);

  String html = htmlPage();
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

  loadRelayConfig(cfg);

  pinMode(cfg.relayPin, OUTPUT);
  setRelay(false);

  if (cfg.useUltrasonic) {
    pinMode(cfg.trigPin, OUTPUT);
    pinMode(cfg.echoPin, INPUT);
  }

  if (cfg.useWebServer) {
    WiFiManager wm;
    wm.setConfigPortalTimeout(180);
    wm.autoConnect("ESP32-Relay");
    server = new WiFiServer(80);
    server->begin();
    Serial.println("[web] Servidor HTTP en puerto 80");
  }

  if (cfg.useAutoCycle) cycleStartTime = millis();
  Serial.println("[actuador_relay] Listo");
}

void loop() {
  if (server) handleClient();

  unsigned long now = millis();

  if (cfg.useAutoCycle) {
    bool target = ((now - cycleStartTime) % (cfg.cycleOnMs + cfg.cycleOffMs)) < cfg.cycleOnMs;
    if (target != relayState) setRelay(target);
  }

  if (cfg.useUltrasonic && !cfg.useAutoCycle) {
    long dist = measureDistance();
    if (dist < cfg.triggerDistanceCm && dist > 0) {
      if (!relayState) {
        setRelay(true);
        lastTriggerTime = now;
      }
    }
    if (relayState && now - lastTriggerTime >= cfg.triggerDurationMs) {
      setRelay(false);
    }
    delay(200);
  } else {
    delay(50);
  }
}
