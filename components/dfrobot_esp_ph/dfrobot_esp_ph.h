#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"
#include "DFRobot_ESP_PH_WITH_ADC.h"
#include "Adafruit_ADS1015.h"

namespace esphome {
namespace dfrobot_esp_ph {

class DFRobotESPPH : public sensor::Sensor, public PollingComponent, public i2c::I2CDevice {
 public:
  void set_temperature_sensor(sensor::Sensor *temp_sensor) { temp_sensor_ = temp_sensor; }
  void set_ph_sensor(sensor::Sensor *ph_sensor) { ph_sensor_ = ph_sensor; }
  void set_adc_channel(int channel) { adc_channel_ = channel; }

  void setup() override;
  void update() override;
  void dump_config() override;

 protected:
  sensor::Sensor *temp_sensor_{nullptr};
  sensor::Sensor *ph_sensor_{nullptr};
  int adc_channel_{0};
  DFRobot_ESP_PH_WITH_ADC ph_;
  Adafruit_ADS1115 ads_;
};

}  // namespace dfrobot_esp_ph
}  // namespace esphome
