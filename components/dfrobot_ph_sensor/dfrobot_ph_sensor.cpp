#include "esphome/core/log.h"
#include "dfrobot_ph_sensor.h"

namespace esphome {
namespace dfrobot_ph_sensor {

static const char *TAG = "dfrobot_ph_sensor";

void DFRobotPHSensor::setup() {
  ph_.begin();
  ads_.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 0.1875mV (default)
  ads_.begin();
  ESP_LOGCONFIG(TAG, "DFRobot pH Sensor initialized");
}

void DFRobotPHSensor::update() {
  if (temp_sensor_ == nullptr || !temp_sensor_->has_state()) {
    ESP_LOGW(TAG, "Temperature sensor not available");
    return;
  }

  float voltage = ads_.readADC_SingleEnded(adc_channel_) * 0.1875 / 1000.0;
  float temperature = temp_sensor_->state;
  float ph_value = ph_.readPH(voltage, temperature);

  ESP_LOGD(TAG, "Voltage: %.2fV, Temperature: %.2f°C, pH: %.2f", voltage, temperature, ph_value);
  publish_state(ph_value);
}

void DFRobotPHSensor::dump_config() {
  ESP_LOGCONFIG(TAG, "DFRobot pH Sensor:");
  LOG_I2C_DEVICE(this);
  LOG_UPDATE_INTERVAL(this);
  LOG_SENSOR("  ", "pH", this);
  ESP_LOGCONFIG(TAG, "  ADC Channel: %d", adc_channel_);
}

}  // namespace dfrobot_ph_sensor
}  // namespace esphome
