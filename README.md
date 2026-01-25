# esphome-shelly_dht

ESPHome **external component** to add support for **Shelly Plus Addon devices with a DHT temperature and humidity sensor**

This repository provides an external ESPHome component so you can easily integrate a DHT sensor connected to a
Shelly Plus Addon device into Home Assistant.

The shelly plus addon has an opto-isolator that only passes each pin in one direction.
This requires the dht component to use two (data) pins instead of one for bidirectional communication.

---

## Features

- Temperature and humidity readings from DHT
- Designed for Shelly Plus Addon devices running ESPHome
- Uses ESPHome external components mechanism
- Fully compatible with Home Assistant

---

## Example ESPHome Configuration

```yaml
esphome:
  name: shelly-dht
  platform: ESP8266
  board: esp01_1m

wifi:
  ssid: "your_ssid"
  password: "your_password"

logger:
api:
ota:

external_components:
  - source:
      type: git
      url: https://github.com/joopmartens/esphome-shelly_dht
    components: [shelly_dht]

sensor:
  - platform: shelly_dht
    pin: GPIO1
    pin_a: GPIO0
    model: DHT22
    update_interval: 60s
    temperature:
      name: "Shelly Temperature"
    humidity:
      name: "Shelly Humidity"
```

---

## Configuration Options

| Option             | Required | Description |
|--------------------|----------|-------------|
| `pin`              | Yes      | GPIO pin DHT data line |
| `pin_a`            | Yes      | 2nd GPIO pin DHT data line |
| `model`            | No       | DHT sensor model (`DHT11`, `DHT22`, `AM2302`) |
| `update_interval`  | No       | Sensor polling interval (default: 60s) |
| `temperature`      | Yes      | Temperature sensor configuration |
| `humidity`         | Yes      | Humidity sensor configuration |

---
