# camara_esp32

Servidor de streaming para ESP32-CAM (AI-Thinker) con WiFiManager.

## Características

- Streaming MJPEG en tiempo real
- Página web embebida con visor
- WiFiManager con captive portal
- Configuración automática de cámara OV2640
- Soporte para flash LED

## Uso

```bash
pio run --target upload
pio device monitor
```

Conectar a `http://<ip>` desde el navegador.

## Sketches legacy unificados

| Sketch | Estado anterior |
|--------|-----------------|
| `camara_web_server_esp_32_cam` | Funcional, WiFi hardcodeado |
| `CameraWebServer` | Clon con credenciales diferentes |
