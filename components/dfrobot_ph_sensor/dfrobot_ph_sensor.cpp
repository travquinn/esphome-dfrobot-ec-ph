#include "dfrobot_ph_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dfrobot_ph_sensor {

static const char *TAG = "dfrobot_ph_sensor";

void DFRobotPHSensor::setup() {
  // Initialize the pH sensor
  ph_.begin();

  // Initialize the ADS1115
  ads_.setGain(GAIN_TWOTHIRDS); // Adjust gain as needed
  ads_.begin();
}

void DFRobotPHSensor::update() {
  // Check if the temperature sensor has a valid state
  if (temp_sensor_ == nullptr || !temp_sensor_->has_state()) {
    ESP_LOGW(TAG, "Temperature sensor not available");
    return;
  }

  // Read voltage from the specified ADC channel
  float voltage = ads_.readADC_SingleEnded(adc_channel_) * 0.1875; // Convert to volts (0.1875 mV per bit for gain of +/-6.144V)
  
  // Read the temperature from the temperature sensor
  float temperature = temp_sensor_->state;

  // Calculate pH value using the read voltage and temperature
  float ph_value = ph_.readPH(voltage, temperature);

  // Publish the pH value to the state
  publish_state(ph_value);
}

void DFRobotPHSensor::dump_config() {
  ESP_LOGCONFIG(TAG, "DFRobot pH Sensor:");
  LOG_I2C_DEVICE(this);
  LOG_UPDATE_INTERVAL(this);
}

}  // namespace dfrobot_ph_sensor
}  // namespace esphome
