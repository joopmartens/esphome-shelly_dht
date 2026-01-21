import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import sensor
from esphome.const import (
    CONF_HUMIDITY,
    CONF_ID,
    CONF_PIN,
    CONF_MODEL,
    CONF_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
    UNIT_PERCENT,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_HUMIDITY,
)

shelly_dht_ns = cg.esphome_ns.namespace("shelly_dht")
ShellyDHT = shelly_dht_ns.class_("ShellyDHT", cg.PollingComponent)

# Import the Model enum from the core DHT component
DHTModel = cg.esphome_ns.enum("dht::DHTModel")
DHT_MODELS = {
    "DHT11": DHTModel.DHT_MODEL_DHT11,
    "DHT22": DHTModel.DHT_MODEL_DHT22,
    "AM2302": DHTModel.DHT_MODEL_AM2302,
    "RHT03": DHTModel.DHT_MODEL_RHT03,
    "SI7021": DHTModel.DHT_MODEL_SI7021,
}

CONF_PIN_A = "pin_a"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(ShellyDHT),
    cv.Required(CONF_PIN): pins.internal_gpio_input_pin_schema,
    cv.Required(CONF_PIN_A): pins.internal_gpio_output_pin_schema,
    cv.Optional(CONF_MODEL, default="DHT22"): cv.enum(DHT_MODELS, upper=True),
    cv.Required(CONF_TEMPERATURE): sensor.sensor_schema(
        unit_of_measurement=UNIT_CELSIUS,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_TEMPERATURE,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    cv.Required(CONF_HUMIDITY): sensor.sensor_schema(
        unit_of_measurement=UNIT_PERCENT,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_HUMIDITY,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
}).extend(cv.polling_component_schema("60s"))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    cg.add(var.set_model(config[CONF_MODEL]))
    
    pin = await cg.gpio_pin_expression(config[CONF_PIN])
    cg.add(var.set_pin(pin))
    
    pin_a = await cg.gpio_pin_expression(config[CONF_PIN_A])
    cg.add(var.set_pin_a(pin_a))

    if CONF_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATURE])
        cg.add(var.set_temperature_sensor(sens))
    if CONF_HUMIDITY in config:
        sens = await sensor.new_sensor(config[CONF_HUMIDITY])
        cg.add(var.set_humidity_sensor(sens))