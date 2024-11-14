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

  void setup() override {
    ph_.begin();
    ads_.setGain(GAIN_ONE);
    ads_.begin();
  }

  void update() override {
    float voltage = ads_.readADC_SingleEnded(adc_channel_) / 10.0f;
    float temperature = temp_sensor_->state;
    float ph_value = ph_.readPH(voltage, temperature);

    ph_sensor_->publish_state(ph_value);
  }

  void dump_config() override {
    ESP_LOGCONFIG(TAG, "DFRobot ESP PH Sensor:");
    LOG_I2C_DEVICE(this);
    LOG_UPDATE_INTERVAL(this);
    LOG_SENSOR("  ", "Temperature", temp_sensor_);
    LOG_SENSOR("  ", "pH", ph_sensor_);
  }

 protected:
  sensor::Sensor *temp_sensor_{nullptr};
  sensor::Sensor *ph_sensor_{nullptr};
  int adc_channel_{0};
  DFRobot_ESP_PH_WITH_ADC ph_;
  Adafruit_ADS1115 ads_;
};

}  // namespace dfrobot_esp_ph
}  // namespace esphome
