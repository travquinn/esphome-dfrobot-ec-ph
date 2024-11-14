#include "dfrobot_ph_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dfrobot_ph_sensor {

static const char *TAG = "dfrobot_ph_sensor";

void DFRobotPHSensor::setup() {
  ph_.begin();
  ads_->setGain(GAIN_TWOTHIRDS); // Use the pointer to access methods
  ads_->begin();
}

void DFRobotPHSensor::update() {
  if (temp_sensor_ == nullptr || !temp_sensor_->has_state()) {
    ESP_LOGW(TAG, "Temperature sensor not available");
    return;
  }

  float voltage = ads_->readADC_SingleEnded(adc_channel_) * 0.1875; // Adjust based on your ADC settings
  float temperature = temp_sensor_->state;
  float ph_value = ph_.readPH(voltage, temperature);

  publish_state(ph_value);
}

void DFRobotPHSensor::dump_config() {
  ESP_LOGCONFIG(TAG, "DFRobot pH Sensor:");
  LOG_I2C_DEVICE(this);
}

}  // namespace dfrobot_ph_sensor
}  // namespace esphome
