// utilidades_esp32
// Selecciona la utilidad descomentando el #define correspondiente
// #define UTIL_IP_PUBLICA
// #define UTIL_TERMOMETRO_WEB
// #define UTIL_LCD_I2C

#include <WiFi.h>
#include <WiFiManager.h>

// ============================================================
// UTILIDAD 1: Obtener IP pública via api.ipify.org
// ============================================================
#ifdef UTIL_IP_PUBLICA

#include <HTTPClient.h>

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFiManager wm;
  wm.setConfigPortalTimeout(120);
  wm.autoConnect("ESP32-Utils");

  Serial.println("[ip] Local IP: " + WiFi.localIP().toString());

  HTTPClient http;
  http.begin("https://api.ipify.org");
  int code = http.GET();
  if (code == HTTP_CODE_OK) {
    Serial.println("[ip] Public IP: " + http.getString());
  } else {
    Serial.printf("[ip] Error: %d\n", code);
  }
  http.end();
}

void loop() { delay(10000); }

#endif

// ============================================================
// UTILIDAD 2: Termometro web con DS18B20
// ============================================================
#ifdef UTIL_TERMOMETRO_WEB

#include <OneWire.h>
#include <DallasTemperature.h>
#include <WebServer.h>

#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
WebServer server(80);

void handleRoot() {
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'><meta name='viewport' content='width=device-width'>";
  html += "<meta http-equiv='refresh' content='5'>";
  html += "<style>body{font-family:sans-serif;text-align:center;padding:2em}</style>";
  html += "</head><body>";
  html += "<h1>Termometro DS18B20</h1>";
  if (temp == DEVICE_DISCONNECTED_C) {
    html += "<p style='color:red'>Sensor desconectado</p>";
  } else {
    html += "<p style='font-size:3em'>" + String(temp, 1) + " &deg;C</p>";
  }
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  sensors.begin();

  WiFiManager wm;
  wm.setConfigPortalTimeout(120);
  wm.autoConnect("ESP32-Termo");

  server.on("/", handleRoot);
  server.begin();
  Serial.println("[termo] http://" + WiFi.localIP().toString());
}

void loop() { server.handleClient(); }

#endif

// ============================================================
// UTILIDAD 3: LCD I2C con Firmata-style serial
// ============================================================
#ifdef UTIL_LCD_I2C

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("ESP32 LCD I2C");
  lcd.setCursor(0, 1);
  lcd.print("Listo");
  Serial.println("[lcd] Escribe texto para enviar al LCD:");
}

void loop() {
  if (Serial.available()) {
    String s = Serial.readStringUntil('\n');
    s.trim();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(s.substring(0, 16));
    if (s.length() > 16) {
      lcd.setCursor(0, 1);
      lcd.print(s.substring(16, 32));
    }
  }
}

#endif

// Si no se define ninguna utilidad, compilar no hace nada
#ifndef UTIL_IP_PUBLICA
#ifndef UTIL_TERMOMETRO_WEB
#ifndef UTIL_LCD_I2C
void setup() { Serial.begin(115200); Serial.println("[utils] Define una utilidad: UTIL_IP_PUBLICA, UTIL_TERMOMETRO_WEB o UTIL_LCD_I2C"); }
void loop() { delay(10000); }
#endif
#endif
#endif
