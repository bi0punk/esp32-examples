# display_oled_esp32

Gestor de displays para ESP32. Soporta OLED SSD1306, TFT LCD 2.4" y displays de 7 segmentos con múltiples modos visuales.

## Modos

| Modo | Valor | Descripción |
|------|-------|-------------|
| Bitmap | 0 | Muestra arte pixelado (Pikachu, Saitama) desde PROGMEM |
| Effects | 1 | Demos de figuras geométricas animadas |
| Scroll | 2 | Texto con desplazamiento horizontal |
| Sensor | 3 | Muestra temperatura/humedad (DHT) |
| Sismo | 4 | Consulta API sísmica y muestra datos |
| TFT BMP | 5 | Muestra imagen BMP desde SD en TFT |
| 7-seg | 6 | Control de display 7 segmentos |

## Hardware soportado

- **OLED** SSD1306 128x64 (I2C) — modos 0-4
- **TFT** 2.4" con SD — modo 5
- **7-seg** 1 o 4 dígitos — modo 6

## Configuración

Editar `data/config.json`:

```json
{
  "displayType": 0,
  "mode": 1,
  "scrollText": "Hola ESP32"
}
```

## Sketches legacy unificados

| Sketch | Estado anterior |
|--------|-----------------|
| `alvaro_henriquez` | Bitmap estático OLED |
| `efectos_oled` | Demo efectos |
| `pikahcu` | Bitmap Pikachu |
| `saitama` | Bitmap Saitama |
| `texto_scroll` | Scroll de texto |
| `tutorial` | Demo básico OLED |
| `interfaz_oled` | OLED + WiFi + NTP + sismos |
| `oled_esp8266` | Credenciales placeholder |
| `sketch_apr1a` | Sismos API (233 líneas) |
| `tft-img` | TFT + SD BMP |
| `sketch_jun13a` | 7-seg loop vacío |
| `sketch_jun13b` | 7-seg + serial |
| `sketch_jun13c` | 7-seg 4-dígitos (.h faltantes) |
