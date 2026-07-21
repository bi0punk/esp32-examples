# actuador_relay_esp32

Control de relés para ESP32 con servidor web, sensor ultrasónico y ciclo automático.

## Modos de operación

| Modo | Descripción |
|------|-------------|
| **Web** | Control manual via HTTP (on/off) con página HTML |
| **Ultrasónico** | Activa relay cuando distancia < umbral |
| **Ciclo** | ON/OFF automático con tiempos configurables |

## Configuración

Editar `data/config.json`:

```json
{
  "relayPin": 16,
  "useUltrasonic": true,
  "trigPin": 17,
  "echoPin": 18,
  "triggerDistanceCm": 100,
  "triggerDurationMs": 3000
}
```

## Sketches legacy unificados

| Sketch | Estado anterior |
|--------|-----------------|
| `relay` | ESP8266 web relay básico |
| `sketch_dec28a` | Ethernet relay |
| `sketch_oct14a` | Relay toggle básico (19 líneas) |
| `wemos_relay_on_off` | Relay + ultrasonido funcional |
