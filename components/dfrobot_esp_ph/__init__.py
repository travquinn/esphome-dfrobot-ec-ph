import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, i2c
from esphome.const import CONF_ID, UNIT_CELSIUS, ICON_THERMOMETER, UNIT_PH, ICON_WATER, CONF_UPDATE_INTERVAL

DEPENDENCIES = ['i2c']
AUTO_LOAD = ['sensor']

CONF_TEMPERATURE_SENSOR = 'temperature_sensor'
CONF_PH_SENSOR = 'ph_sensor'
CONF_ADC_CHANNEL = 'adc_channel'

dfrobot_esp_ph_ns = cg.esphome_ns.namespace('dfrobot_esp_ph')
DFRobotESPPH = dfrobot_esp_ph_ns.class_('DFRobotESPPH', sensor.Sensor, cg.PollingComponent, i2c.I2CDevice)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(DFRobotESPPH),
    cv.Required(CONF_TEMPERATURE_SENSOR): cv.use_id(sensor.Sensor),
    cv.Required(CONF_PH_SENSOR): sensor.sensor_schema(
        unit_of_measurement=UNIT_PH,
        icon=ICON_WATER,
        accuracy_decimals=2
    ),
    cv.Required(CONF_ADC_CHANNEL): cv.int_range(min=0, max=3),
    cv.Optional(CONF_UPDATE_INTERVAL, default='60s'): cv.update_interval,
}).extend(cv.COMPONENT_SCHEMA).extend(i2c.i2c_device_schema(0x48))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    temp_sens = await cg.get_variable(config[CONF_TEMPERATURE_SENSOR])
    cg.add(var.set_temperature_sensor(temp_sens))
    
    ph_sens = await sensor.new_sensor(config[CONF_PH_SENSOR])
    cg.add(var.set_ph_sensor(ph_sens))
    
    cg.add(var.set_adc_channel(config[CONF_ADC_CHANNEL]))

    if CONF_UPDATE_INTERVAL in config:
        cg.add(var.set_update_interval(config[CONF_UPDATE_INTERVAL]))
