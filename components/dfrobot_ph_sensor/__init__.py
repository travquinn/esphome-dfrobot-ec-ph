# custom_components/ph_sensor/__init__.py
from homeassistant.components.sensor import PLATFORM_SCHEMA, SensorEntity
import homeassistant.helpers.config_validation as cv

PLATFORM_SCHEMA = cv.extend({
    cv.Required('name'): cv.string,
})

def setup_platform(hass, config, add_entities, discovery_info=None):
    sensor_name = config.get('name')
    
    class PhSensor(SensorEntity):
        def __init__(self, name):
            self._name = name
            self._state = None
        
        @property
        def name(self):
            return self._name
        
        @property
        def state(self):
            return self._state
        
        @property
        def state_class(self):
            return 'measurement'
        
        @property
        def unit_of_measurement(self):
            return 'pH'
    
    add_entities([PhSensor(sensor_name)])
