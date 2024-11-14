#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/ads1115_adc/ads1115_adc.h"

namespace esphome {
namespace dfrobot_ads1115_ph {

class CalibrationSwitch : public switch_::Switch, public Component {
 public:
  void set_parent(Component *parent) { parent_ = parent; }
  void write_state(bool state) override;
 protected:
  Component *parent_{nullptr};
};

class DFRobotADS1115PH : public PollingComponent {
 public:
  void set_ads1115(ads1115_adc::ADS1115_ADC *ads) { ads1115_ = ads; }
  void set_channel(uint8_t channel) { channel_ = channel; }
  void set_acid_voltage(float voltage) { acid_voltage_ = voltage; }
  void set_neutral_voltage(float voltage) { neutral_voltage_ = voltage; }
  void set_temperature(float temp) { temperature_ = temp; }
  void set_ph_sensor(sensor::Sensor *sens) { ph_sensor_ = sens; }
  void set_calibration_switch(CalibrationSwitch *sw) { calibration_switch_ = sw; }

  void setup() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

 protected:
  ads1115_adc::ADS1115_ADC *ads1115_{nullptr};
  sensor::Sensor *ph_sensor_{nullptr};
  CalibrationSwitch *calibration_switch_{nullptr};
  uint8_t channel_{0};
  float acid_voltage_{0};
  float neutral_voltage_{0};
  float temperature_{25};
  float ph_value_{0};

  float read_ph(float voltage);
};

}  // namespace dfrobot_ads1115_ph
}  // namespace esphome
