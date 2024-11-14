import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, i2c
from esphome.const import CONF_ID, UNIT_CELSIUS, ICON_THERMOMETER, UNIT_PH, ICON_WATER

DEPENDENCIES = ['i2c']
AUTO_LOAD = ['sensor']

dfrobot_esp_ph_ns = cg.esphome_ns.namespace('dfrobot_esp_ph')
DFRobotESPPH = dfrobot_esp_ph_ns.class_('DFRobotESPPH', cg.PollingComponent, i2c.I2CDevice)

CONF_TEMPERATURE_SENSOR = 'temperature_sensor'
CONF_PH_SENSOR = 'ph_sensor'
CONF_ADC_CHANNEL = 'adc_channel'

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(DFRobotESPPH),
    cv.Required(CONF_TEMPERATURE_SENSOR): sensor.sensor_schema(
        unit_of_measurement=UNIT_CELSIUS,
        icon=ICON_THERMOMETER,
        accuracy_decimals=1
    ),
    cv.Required(CONF_PH_SENSOR): sensor.sensor_schema(
        unit_of_measurement=UNIT_PH,
        icon=ICON_WATER,
        accuracy_decimals=2
    ),
    cv.Required(CONF_ADC_CHANNEL): cv.int_range(min=0, max=3)
}).extend(cv.polling_component_schema('60s')).extend(i2c.i2c_device_schema(0x48))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    temp_sens = await sensor.new_sensor(config[CONF_TEMPERATURE_SENSOR])
    cg.add(var.set_temperature_sensor(temp_sens))
    
    ph_sens = await sensor.new_sensor(config[CONF_PH_SENSOR])
    cg.add(var.set_ph_sensor(ph_sens))
    
    cg.add(var.set_adc_channel(config[CONF_ADC_CHANNEL]))
