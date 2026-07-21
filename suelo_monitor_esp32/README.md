# suelo_monitor_esp32

Monitor de humedad de suelo para ESP32. Soporta sensor capacitivo, control de relés, NTP, servidor web y envío HTTP.

## Modos de salida

- **Serial** — monitoreo por puerto serie
- **Web Server** — página HTML con humedad en tiempo real
- **HTTP POST** — envía JSON a endpoint REST
- **Relés** — control de hasta 4 relés por pin configurable

## Configuración

Editar `data/config.json` y subir a SPIFFS:

```bash
pio run --target uploadfs
```

| Campo | Descripción | Default |
|-------|-------------|---------|
| `sensorPin` | Pin ADC del sensor capacitivo | 34 |
| `dryValue` | Valor ADC en suelo seco | 1000 |
| `wetValue` | Valor ADC en suelo mojado | 400 |
| `sampleIntervalSec` | Intervalo entre lecturas | 60 |
| `relayPins` | Pines de relés (array) | [] |
| `useWebServer` | Habilita servidor web en puerto 80 | true |
| `useNTP` | Sincroniza hora con NTP Chile | true |

## Sketches legacy unificados

| Sketch | Estado anterior |
|--------|-----------------|
| `garden` | Ethernet Shield, IP fija |
| `garden2` | Ethernet + MySQL hardcodeado |
| `humedad_web_2` | EthernetUdp mal usado |
| `humedad_web_wifi` | Archivo truncado |
| `humedad_web` | Calibración rota (min=max=0) |
| `sketch_nov28a` | ESP8266 básico |
| `sketch_nov29a` | Ethernet, hora hardcodeada |
| `sketch_oct16a` | ESP8266 funcional básico |
