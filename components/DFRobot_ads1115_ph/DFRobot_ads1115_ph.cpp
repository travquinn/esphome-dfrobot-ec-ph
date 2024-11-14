#include "DFRobot_ads1115_ph.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dfrobot_ads1115_ph {

static const char *const TAG = "dfrobot_ads1115_ph";

void CalibrationSwitch::write_state(bool state) {
  this->publish_state(state);
}

void DFRobotADS1115PH::setup() {
  this->calibration_switch_->set_parent(this);
}

void DFRobotADS1115PH::update() {
  float voltage = this->ads1115_->read_voltage(this->channel_);
  
  if (this->calibration_switch_->state) {
    ESP_LOGD(TAG, "Calibration mode: Voltage = %.2f", voltage);
    return;
  }

  this->ph_value_ = this->read_ph(voltage);
  this->ph_sensor_->publish_state(this->ph_value_);
}

void DFRobotADS1115PH::dump_config() {
  ESP_LOGCONFIG(TAG, "DFRobot ADS1115 pH Sensor:");
  ESP_LOGCONFIG(TAG, "  Channel: %d", this->channel_);
  ESP_LOGCONFIG(TAG, "  Acid Voltage: %.2f", this->acid_voltage_);
  ESP_LOGCONFIG(TAG, "  Neutral Voltage: %.2f", this->neutral_voltage_);
  ESP_LOGCONFIG(TAG, "  Temperature: %.2f", this->temperature_);
}

float DFRobotADS1115PH::read_ph(float voltage) {
  float slope = (7.0 - 4.0) / ((this->neutral_voltage_ - 1500.0) / 3.0 - (this->acid_voltage_ - 1500.0) / 3.0);
  float intercept = 7.0 - slope * (this->neutral_voltage_ - 1500.0) / 3.0;
  return slope * (voltage - 1500.0) / 3.0 + intercept;
}

}  // namespace dfrobot_ads1115_ph
}  // namespace esphome
