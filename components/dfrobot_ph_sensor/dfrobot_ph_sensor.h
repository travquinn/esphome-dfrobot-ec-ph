// dfrobot_ph_sensor.h
#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace dfrobot_ph_sensor {

class DFRobotPHSensor : public Component, public i2c::I2CDevice {
 public:
  void setup() override;
  void update() override;

  void set_adc_channel(uint8_t adc_channel) { adc_channel_ = adc_channel; }
  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }

 protected:
  uint8_t adc_channel_{0};
  sensor::Sensor *temperature_sensor_;
};

}  // namespace dfrobot_ph_sensor
}  // namespace esphome
