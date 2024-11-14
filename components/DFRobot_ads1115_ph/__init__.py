import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, switch
from esphome.const import (
    CONF_ID,
    CONF_UPDATE_INTERVAL,
    UNIT_PH,
    DEVICE_CLASS_PH,
    STATE_CLASS_MEASUREMENT,
)

DEPENDENCIES = ['ads1115_adc']
AUTO_LOAD = ["switch", "sensor"]

CONF_ADS1115_ID = "id_ads1115"
CONF_CHANNEL = "channel"
CONF_ACID_VOLTAGE = "acid_voltage"
CONF_NEUTRAL_VOLTAGE = "neutral_voltage"
CONF_TEMPERATURE = "temperature"
CONF_CALIBRATION_MODE = "calibration_mode"
CONF_PH_SENSOR = "ph_sensor"

dfrobot_ads1115_ph_ns = cg.esphome_ns.namespace("dfrobot_ads1115_ph")
DFRobotADS1115PH = dfrobot_ads1115_ph_ns.class_("DFRobotADS1115PH", cg.Component)
CalibrationSwitch = dfrobot_ads1115_ph_ns.class_("CalibrationSwitch", switch.Switch)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(DFRobotADS1115PH),
    cv.Required(CONF_ADS1115_ID): cv.use_id(cg.Component),
    cv.Required(CONF_CHANNEL): cv.int_range(min=0, max=3),
    cv.Required(CONF_ACID_VOLTAGE): cv.float_,
    cv.Required(CONF_NEUTRAL_VOLTAGE): cv.float_,
    cv.Optional(CONF_TEMPERATURE, default=25.0): cv.float_,
    cv.Optional(CONF_UPDATE_INTERVAL, default="60s"): cv.update_interval,
    cv.Required(CONF_CALIBRATION_MODE): switch.switch_schema(CalibrationSwitch),
    cv.Required(CONF_PH_SENSOR): sensor.sensor_schema(
        unit_of_measurement=UNIT_PH,
        accuracy_decimals=2,
        device_class=DEVICE_CLASS_PH,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    ads1115 = await cg.get_variable(config[CONF_ADS1115_ID])
    cg.add(var.set_ads1115(ads1115))
    cg.add(var.set_channel(config[CONF_CHANNEL]))
    cg.add(var.set_acid_voltage(config[CONF_ACID_VOLTAGE]))
    cg.add(var.set_neutral_voltage(config[CONF_NEUTRAL_VOLTAGE]))
    cg.add(var.set_temperature(config[CONF_TEMPERATURE]))

    sens = await sensor.new_sensor(config[CONF_PH_SENSOR])
    cg.add(var.set_ph_sensor(sens))

    cal_switch = await switch.new_switch(config[CONF_CALIBRATION_MODE])
    cg.add(var.set_calibration_switch(cal_switch))

    cg.add(var.set_update_interval(config[CONF_UPDATE_INTERVAL]))
