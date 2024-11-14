import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID

DEPENDENCIES = ['dfrobot_esp_ph']

dfrobot_esp_ph_ns = cg.esphome_ns.namespace('dfrobot_esp_ph')
DFRobotESPPH = dfrobot_esp_ph_ns.class_('DFRobotESPPH', sensor.Sensor, cg.PollingComponent)

CONFIG_SCHEMA = sensor.sensor_schema().extend({
    cv.GenerateID(): cv.declare_id(DFRobotESPPH),
})

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await sensor.register_sensor(var, config)
