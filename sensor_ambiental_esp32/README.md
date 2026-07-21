# sensor_ambiental_esp32

Sensor ambiental unificado para ESP32. Soporta múltiples sensores de temperatura, humedad y gas en un solo firmware configurable vía JSON.

## Sensores soportados

| Tipo | Modelos | Pines por defecto |
|------|---------|-------------------|
| DHT | DHT11, DHT22 | GPIO4 |
| SHT | SHT31 (I2C 0x44) | I2C default |
| SHTC3 | SHTC3 (I2C) | I2C default |
| MQ | MQ-135 | GPIO34 (ADC) |
| OLED | SSD1306 128x64 | I2C (SDA=21, SCL=22) |

## Modos de salida

- **Serial** — monitor serie a 115200 baud
- **HTTP POST** — envía JSON a endpoint REST
- **OLED** — muestra en pantalla SSD1306

## Configuración

Editar `data/config.json` y subir a SPIFFS:

```bash
pio run --target uploadfs
```

O editar `src/config.h` para valores por defecto compilados.

## WiFi

Usa WiFiManager con captive portal. Si no hay WiFi configurado, abre AP `ESP32-Sensor` para configuración vía web.

## Sketches legacy unificados

Este proyecto reemplaza los siguientes sketches del Arduino IDE:

| Sketch legacy | Estado anterior |
|---|---|
| `hu_edad_ytemp` | Roto (DHT11 en pin analógico) |
| `humedad_temp_wemos` | Solo serial |
| `sketch_dec21a` | Solo serial |
| `touch_tft` | Loop vacío, nombre engañoso |
| `api_gas_esp32` | Solo MQ-135, credenciales hardcodeadas |
| `humedad_api` | Solo un sensor |
| `dht22test_wemos_json_web_server` | Placeholders, ESP8266 |
| `hyt_dig` | Solo SHTC3+OLED |
| `qqqqqqqqqqqqqq2222222222` | Nombre basura, dual DHT |
| `qqqqqqqqqqqqqqqqqqqqq` | Nombre basura, single DHT |
