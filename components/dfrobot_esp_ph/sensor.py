import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import CONF_ID, ICON_WATER, UNIT_PH

DEPENDENCIES = ["i2c"]
CONF_TEMPERATURE_SENSOR = "temperature_sensor"
CONF_ADC_CHANNEL = "adc_channel"

dfrobot_ph_sensor_ns = cg.esphome_ns.namespace("dfrobot_ph_sensor")
DFRobotPHSensor = dfrobot_ph_sensor_ns.class_(
    "DFRobotPHSensor", cg.PollingComponent, i2c.I2CDevice, sensor.Sensor
)

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        DFRobotPHSensor,
        unit_of_measurement=UNIT_PH,
        icon=ICON_WATER,
        accuracy_decimals=2,
    )
    .extend({
        cv.Required(CONF_TEMPERATURE_SENSOR): cv.use_id(sensor.Sensor),
        cv.Required(CONF_ADC_CHANNEL): cv.int_range(min=0, max=3),
    })
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(0x48))
)

async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    temp_sens = await cg.get_variable(config[CONF_TEMPERATURE_SENSOR])
    cg.add(var.set_temperature_sensor(temp_sens))
    cg.add(var.set_adc_channel(config[CONF_ADC_CHANNEL]))
