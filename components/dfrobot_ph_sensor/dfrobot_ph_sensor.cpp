#include "dfrobot_ph_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dfrobot_ph_sensor {

static const char *TAG = "dfrobot_ph_sensor";

void DFRobotPHSensor::setup() {
  ph_.begin();
  ads_.setGain(GAIN_TWOTHIRDS);
  ads_.begin();
}

void DFRobotPHSensor::update() {
  if (temp_sensor_ == nullptr || !temp_sensor_->has_state()) {
    ESP_LOGW(TAG, "Temperature sensor not available");
    return;
  }

  float voltage = ads_.readADC_SingleEnded(adc_channel_) * 0.1875 / 1000.0;
  float temperature = temp_sensor_->state;
  float ph_value = ph_.readPH(voltage, temperature);

  publish_state(ph_value);
  ESP_LOGD(TAG, "pH: %.2f, Voltage: %.2f, Temperature: %.2f", ph_value, voltage, temperature);
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
