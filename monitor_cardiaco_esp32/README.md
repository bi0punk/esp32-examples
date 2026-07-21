# monitor_cardiaco_esp32

Monitor de frecuencia cardíaca con sensor AD8232 para ESP32.

## Hardware

| Pin | Conexión |
|-----|----------|
| 8 | LO+ (detección electrodos) |
| 9 | LO- (detección electrodos) |
| 10 | SHUTDOWN (standby) |
| A0 | OUTPUT (señal ECG) |

## Salida serial

```
ECG:45 HR:72 HRV:0.123
```

## Sketches legacy unificados

| Sketch | Problemas |
|--------|-----------|
| `heart` | Pines inconsistentes (setup vs loop usaban distintos pines) |
| `sketch_oct16b` | Funcional, sin modularizar |
