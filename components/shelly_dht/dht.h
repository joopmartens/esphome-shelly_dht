#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace shelly_dht {

// Define model enum locally to avoid dependency errors
enum ShellyDHTModel {
  MODEL_DHT11 = 0,
  MODEL_DHT22 = 1,
  MODEL_AM2302 = 2,
  MODEL_RHT03 = 3,
  MODEL_SI7021 = 4
};

class ShellyDHT : public PollingComponent {
 public:
  void set_pin(InternalGPIOPin *pin) { pin_ = pin; }
  void set_pin_a(InternalGPIOPin *pin_a) { pin_a_ = pin_a; }
  void set_model(ShellyDHTModel model) { model_ = model; }
  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }
  void set_humidity_sensor(sensor::Sensor *humidity_sensor) { humidity_sensor_ = humidity_sensor; }

  void setup() override {
    this->pin_->setup();
    this->pin_a_->setup();
    this->pin_a_->digital_write(true); // Pull high (idle state)
  }

  void dump_config() override {
    ESP_LOGCONFIG("shelly_dht", "Shelly DHT:");
    LOG_PIN("  Input Pin: ", this->pin_);
    LOG_PIN("  Output Pin (A): ", this->pin_a_);
    const char *model_s = "Unknown";
    switch (this->model_) {
      case MODEL_DHT11: model_s = "DHT11"; break;
      case MODEL_DHT22: model_s = "DHT22"; break;
      case MODEL_AM2302: model_s = "AM2302"; break;
      case MODEL_RHT03: model_s = "RHT03"; break;
      case MODEL_SI7021: model_s = "SI7021"; break;
    }
    ESP_LOGCONFIG("shelly_dht", "  Model: %s", model_s);
  }

  void update() override {
    uint8_t data[5] = {0, 0, 0, 0, 0};
    if (this->read_sensor_(data)) {
        float temperature, humidity;
        
        if (this->model_ == MODEL_DHT11) {
            humidity = data[0];
            temperature = data[2];
        } else {
            // DHT22, AM2302, etc.
            humidity = ((uint16_t(data[0]) << 8) | data[1]) * 0.1f;
            temperature = ((uint16_t(data[2] & 0x7F) << 8) | data[3]) * 0.1f;
            if (data[2] & 0x80) temperature *= -1.0f;
        }

        if (this->temperature_sensor_ != nullptr)
            this->temperature_sensor_->publish_state(temperature);
        if (this->humidity_sensor_ != nullptr)
            this->humidity_sensor_->publish_state(humidity);
    } else {
        if (this->temperature_sensor_ != nullptr)
            this->temperature_sensor_->publish_state(NAN);
        if (this->humidity_sensor_ != nullptr)
            this->humidity_sensor_->publish_state(NAN);
    }
  }

 protected:
  // Your existing bit-banging read_sensor_ implementation goes here
  bool read_sensor_(uint8_t *data);

  InternalGPIOPin *pin_;
  InternalGPIOPin *pin_a_;
  ShellyDHTModel model_;
  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *humidity_sensor_{nullptr};
};

}  // namespace shelly_dht
}  // namespace esphome