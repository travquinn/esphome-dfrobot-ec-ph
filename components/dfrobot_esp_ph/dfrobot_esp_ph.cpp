#include "dfrobot_esp_ph.h"

namespace esphome {
namespace dfrobot_esp_ph {

void DFRobotESPPH::setup() {
  ph_.begin();
  ads_.setGain(GAIN_ONE);
  ads_.begin();
}

void DFRobotESPPH::update() {
  if (temp_sensor_->has_state() && ph_sensor_ != nullptr) {
    float voltage = ads_.readADC_SingleEnded(adc_channel_) / 10.0f;
    float temperature = temp_sensor_->state;
    float ph_value = ph_.readPH(voltage, temperature);

    ph_sensor_->publish_state(ph_value);
  } else {
    ESP_LOGW("dfrobot_esp_ph", "Temperature sensor has no state or pH sensor is not set");
  }
}

void DFRobotESPPH::dump_config() {
  ESP_LOGCONFIG("dfrobot_esp_ph", "DFRobot ESP PH Sensor:");
  LOG_I2C_DEVICE(this);
  LOG_UPDATE_INTERVAL(this);
  LOG_SENSOR("  ", "Temperature", temp_sensor_);
  LOG_SENSOR("  ", "pH", ph_sensor_);
  ESP_LOGCONFIG("dfrobot_esp_ph", "  ADC Channel: %d", adc_channel_);
}

}  // namespace dfrobot_esp_ph
}  // namespace esphome
