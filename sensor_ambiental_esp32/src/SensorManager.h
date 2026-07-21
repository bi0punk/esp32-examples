#pragma once

#include <DHT.h>
#include <Adafruit_SHT31.h>
#include <Adafruit_SHTC3.h>
#include "config.h"

struct SensorReading {
  float temperature = NAN;
  float humidity = NAN;
  float gasRaw = NAN;
  float gasVoltage = NAN;
};

class SensorManager {
public:
  SensorManager(const SensorConfig &cfg) : cfg_(cfg) {}

  bool begin() {
    bool ok = false;
    if (cfg_.useDHT) {
      int type = (cfg_.dhtType == 11) ? DHT11 : DHT22;
      dht_ = new DHT(cfg_.dhtPin, type);
      dht_->begin();
      ok = true;
    }
    if (cfg_.useSHT31) {
      sht31_ = new Adafruit_SHT31();
      if (!sht31_->begin(0x44)) {
        delete sht31_;
        sht31_ = nullptr;
      } else {
        ok = true;
      }
    }
    if (cfg_.useSHTC3) {
      if (shtc3_.begin() != SHTC3_Status_Nominal) {
        // fallo SHTC3
      } else {
        ok = true;
      }
    }
    if (cfg_.useMQ135) {
      pinMode(cfg_.mq135Pin, INPUT);
      ok = true;
    }
    return ok;
  }

  SensorReading read() {
    SensorReading r;
    if (dht_) {
      r.temperature = dht_->readTemperature();
      r.humidity = dht_->readHumidity();
    }
    if (sht31_) {
      r.temperature = sht31_->readTemperature();
      r.humidity = sht31_->readHumidity();
    }
    if (cfg_.useSHTC3) {
      shtc3_.update();
      r.temperature = shtc3_.toDegC();
      r.humidity = shtc3_.toPercent();
    }
    if (cfg_.useMQ135) {
      r.gasRaw = analogRead(cfg_.mq135Pin);
      r.gasVoltage = r.gasRaw * (3.3f / 4095.0f);
    }
    return r;
  }

private:
  const SensorConfig &cfg_;
  DHT *dht_ = nullptr;
  Adafruit_SHT31 *sht31_ = nullptr;
  SHTC3 shtc3_;
};
