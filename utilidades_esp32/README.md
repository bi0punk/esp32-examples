# utilidades_esp32

Tres utilidades independientes en un solo proyecto. Seleccionar descomentando el `#define` en `src/main.cpp`.

## Utilidades

| #define | Descripción | Hardware |
|---------|-------------|----------|
| `UTIL_IP_PUBLICA` | Obtiene IP pública via api.ipify.org | Ninguno |
| `UTIL_TERMOMETRO_WEB` | Servidor web con sensor DS18B20 | DS18B20 en GPIO4 |
| `UTIL_LCD_I2C` | LCD 16x2 I2C controlado por serial | LCD I2C 0x3F |

## Uso

```cpp
// En src/main.cpp, descomenta una:
#define UTIL_TERMOMETRO_WEB
```

```bash
pio run --target upload
```

## Sketches legacy unificados

| Sketch | Utilidad |
|--------|----------|
| `HelloWorld` | Firmata + LCD I2C |
| `sketch_mar15a` | IP pública via ipify |
| `wifi` | DS18B20 web server |
