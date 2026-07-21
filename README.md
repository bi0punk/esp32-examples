# ESP32 Examples

Colección de proyectos ESP32 refactorizados desde sketches Arduino legacy. Cada proyecto es independiente, con su propio `platformio.ini` y configuración.

## Familias

| Familia | Proyecto | Descripción | Sketches legacy |
|---------|----------|-------------|-----------------|
| **A** | `sensor_ambiental_esp32/` | Sensor multi-tipo (DHT/SHT/MQ) con salida serial, HTTP y OLED | 10 unificados |
| **B** | `suelo_monitor_esp32/` | Monitor de humedad de suelo con backend configurable | 8 unificados |
| **C** | `display_oled_esp32/` | Gestor de displays OLED/TFT/LCD + efectos | 13 unificados |
| **D** | `actuador_relay_esp32/` | Control de relés con sensor de distancia y HTTP | 4 unificados |
| **E** | `camara_esp32/` | Servidor de streaming ESP32-CAM con detección | 2 unificados |
| **F** | `monitor_cardiaco_esp32/` | Monitor de frecuencia cardíaca AD8232 | 2 unificados |
| **G** | `utilidades_esp32/` | Utilidades varias (termómetro web, IP pública, LCD) | 3 unificados |

## Requisitos

- [PlatformIO](https://platformio.org/) (VS Code extension o CLI)
- ESP32 DevKit o compatible
- Sensores según cada proyecto

## Uso

```bash
cd sensor_ambiental_esp32
pio run --target upload
pio run --target uploadfs  # para subir config.json
pio device monitor
```

## Origen

Estos proyectos reemplazan ~45 sketches Arduino IDE legacy del laboratorio, migrados a ESP32 con PlatformIO, configuración externa y código modular.
