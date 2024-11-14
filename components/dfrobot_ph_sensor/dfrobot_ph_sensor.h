#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"
#include "DFRobot_ESP_PH_WITH_ADC.h"
#include <Adafruit_ADS1X15.h>
#include "esphome/components/ads1115/ads1115.h" // Include the ADS1115 component

namespace esphome {
namespace dfrobot_ph_sensor {

class DFRobotPHSensor : public sensor::Sensor, public PollingComponent, public i2c::I2CDevice {
 public:
  void set_temperature_sensor(sensor::Sensor *temp_sensor) { temp_sensor_ = temp_sensor; }
  void set_adc_channel(int channel) { adc_channel_ = channel; }
  void set_ads1115(ADS1115Component *ads) { ads_ = ads; } // Change this line

  void setup() override;
  void update() override;
  void dump_config() override;

 protected:
  sensor::Sensor *temp_sensor_{nullptr};
  int adc_channel_{0};
  DFRobot_ESP_PH_WITH_ADC ph_;
  ADS1115Component *ads_; // Change this to point to ADS1115Component
};

}  // namespace dfrobot_ph_sensor
}  // namespace esphome
