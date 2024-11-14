import esphome.codegen as cg
from esphome.components import sensor
import esphome.config_validation as cv
from esphome.const import CONF_ID

DEPENDENCIES = ['dfrobot_esp_ph']

dfrobot_esp_ph_ns = cg.esphome_ns.namespace('dfrobot_esp_ph')
DFRobotESPPH = dfrobot_esp_ph_ns.class_('DFRobotESPPH', sensor.Sensor, cg.PollingComponent)

CONFIG_SCHEMA = cv.Schema({})

async def to_code(config):
    pass
