#include <esp_camera.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include "camera_pins.h"

#define CAMERA_MODEL_AI_THINKER

static WiFiServer streamServer(81);
static WiFiClient streamClient;

static camera_config_t cameraConfig() {
  camera_config_t c;
  c.ledc_channel = LEDC_CHANNEL_0;
  c.ledc_timer = LEDC_TIMER_0;
  c.pin_d0 = Y2_GPIO_NUM;
  c.pin_d1 = Y3_GPIO_NUM;
  c.pin_d2 = Y4_GPIO_NUM;
  c.pin_d3 = Y5_GPIO_NUM;
  c.pin_d4 = Y6_GPIO_NUM;
  c.pin_d5 = Y7_GPIO_NUM;
  c.pin_d6 = Y8_GPIO_NUM;
  c.pin_d7 = Y9_GPIO_NUM;
  c.pin_xclk = XCLK_GPIO_NUM;
  c.pin_pclk = PCLK_GPIO_NUM;
  c.pin_vsync = VSYNC_GPIO_NUM;
  c.pin_href = HREF_GPIO_NUM;
  c.pin_sccb_sda = SIOD_GPIO_NUM;
  c.pin_sccb_scl = SIOC_GPIO_NUM;
  c.pin_pwdn = PWDN_GPIO_NUM;
  c.pin_reset = RESET_GPIO_NUM;
  c.xclk_freq_hz = 20000000;
  c.frame_size = FRAMESIZE_QVGA;
  c.pixel_format = PIXFORMAT_JPEG;
  c.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  c.fb_location = CAMERA_FB_IN_PSRAM;
  c.jpeg_quality = 12;
  c.fb_count = 1;
  return c;
}

static void handleStream(WiFiClient &c) {
  String head = "HTTP/1.1 200 OK\r\n"
    "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n"
    "Connection: close\r\n\r\n";
  c.print(head);

  while (c.connected()) {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
      delay(10);
      continue;
    }
    c.printf("--frame\r\n");
    c.printf("Content-Type: image/jpeg\r\n");
    c.printf("Content-Length: %u\r\n\r\n", fb->len);
    c.write(fb->buf, fb->len);
    c.printf("\r\n");
    esp_camera_fb_return(fb);
    delay(30);
  }
}

static void handleRoot(WiFiClient &c) {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'><meta name='viewport' content='width=device-width'>";
  html += "<title>ESP32-CAM</title>";
  html += "<style>body{font-family:sans-serif;text-align:center;padding:2em;background:#111;color:#fff}";
  html += "img{max-width:100%;border:2px solid #333;border-radius:8px}</style></head><body>";
  html += "<h1>ESP32-CAM Stream</h1>";
  html += "<img src='/stream' />";
  html += "<p><small>" + WiFi.localIP().toString() + "</small></p></body></html>";

  c.println("HTTP/1.1 200 OK");
  c.println("Content-Type: text/html");
  c.println("Connection: close");
  c.println();
  c.println(html);
  c.stop();
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  WiFiManager wm;
  wm.setConfigPortalTimeout(180);
  wm.autoConnect("ESP32-CAM");

  camera_config_t config = cameraConfig();
  if (psramFound()) {
    config.jpeg_quality = 10;
    config.fb_count = 2;
    config.grab_mode = CAMERA_GRAB_LATEST;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.fb_location = CAMERA_FB_IN_DRAM;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("[camera] Init error: 0x%x\n", err);
    return;
  }

  sensor_t *s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_QVGA);
  s->set_brightness(s, 1);
  s->set_saturation(s, -2);

#if defined(LED_GPIO_NUM)
  ledcAttach(LED_GPIO_NUM, 5000, 8);
#endif

  streamServer.begin();
  Serial.printf("[camera] Ready! http://%s\n", WiFi.localIP().toString().c_str());
}

void loop() {
  WiFiClient c = streamServer.available();
  if (!c) { delay(10); return; }

  String req = c.readStringUntil('\r');
  c.flush();

  if (req.indexOf("GET /stream") >= 0) {
    handleStream(c);
  } else {
    handleRoot(c);
  }
  delay(10);
}
