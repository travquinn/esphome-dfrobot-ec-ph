# sensor.py
from homeassistant.components.sensor import PLATFORM_SCHEMA, SensorEntity
import homeassistant.helpers.config_validation as cv
from homeassistant.const import DEVICE_CLASS_SENSOR, STATE_UNKNOWN

PLATFORM_SCHEMA = PLATFORM_SCHEMA.extend({
    cv.Required("name"): cv.string,
    cv.Optional("temperature_sensor", default=None): cv.entity_id,
    cv.Required("adc_channel"): cv.positive_int,
    cv.Required("i2c_id"): cv.use_id(i2c.I2CComponent),
})

class DFRobotPHSensor(SensorEntity):
    def __init__(self, name, adc_channel, i2c_id, temperature_sensor):
        self._name = name
        self._adc_channel = adc_channel
        self._i2c_id = i2c_id
        self._temperature_sensor = temperature_sensor
        self._state = None

    @property
    def name(self):
        return self._name

    @property
    def device_class(self):
        return DEVICE_CLASS_SENSOR

    @property
    def state(self):
        return self._state

    async def update(self):
        # Simulate fetching data from the sensor
        adc_value = await self._i2c_id.read_adc(self._adc_channel)
        temperature = await self._temperature_sensor.state
        self._state = calculate_ph(adc_value, temperature)

def setup_platform(hass, config, add_entities, discovery_info=None):
    i2c = i2c.I2CComponent.from_config(config, hass)
    adc_channel = config.get("adc_channel")
    temperature_sensor = sensor.SensorEntity.from_config(config.get("temperature_sensor"), hass)
    
    ph_sensor = DFRobotPHSensor(config.get("name"), adc_channel, i2c, temperature_sensor)
    add_entities([ph_sensor])
