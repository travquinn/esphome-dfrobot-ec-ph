#include "esphome.h"
#include "DFRobot_ESP_PH_WITH_ADC.h"
#include "EEPROM.h"

// ADS 1115
#include "Adafruit_ADS1X15.h"

// DALLAS temperature sensor
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

float voltage, phValue, temperature = 25;

float readTemperature()
{
	//add your code here to get the temperature from your temperature sensor
	sensors.requestTemperatures();
	return sensors.getTempCByIndex(0);
}

class CustomDFRobotPhSensor : public PollingComponent, public Sensor, public CustomAPIDevice {
 public:
  DFRobot_ESP_PH_WITH_ADC ph;
  Adafruit_ADS1115 ads;
  Sensor *ph_sensor = new Sensor();
  Sensor *temperature_sensor = new Sensor();
  Sensor *voltage_sensor = new Sensor();

  // constructor
  CustomDFRobotPhSensor(uint32_t update_interval) : PollingComponent(update_interval) {}
  
  float get_setup_priority() const override { return esphome::setup_priority::IO; }

  void setup() override {
    // This will be called by App.setup()
    EEPROM.begin(32);//needed EEPROM.begin to store calibration k in eeprom
    ph.begin();
    sensors.begin();
    ads.setGain(GAIN_ONE);
    ads.begin();

    // services
    register_service(&CustomDFRobotPhSensor::enter_ph_calibration_mode, "enter_ph");
    register_service(&CustomDFRobotPhSensor::exit_ph_calibration_mode, "exit_ph");
    register_service(&CustomDFRobotPhSensor::calibrate_ph, "calibrate_ph");
    
  }
  void update() override {
    // This will be called every "update_interval" milliseconds.
    voltage = ads.readADC_SingleEnded(0) / 10; // read the voltage
    voltage_sensor->publish_state(voltage);
    temperature = readTemperature();
    temperature_sensor->publish_state(temperature);
    phValue = ph.readPH(voltage, temperature); // convert voltage to pH with temperature compensation
    ph_sensor->publish_state(phValue);
  }
  void enter_ph_calibration_mode() {
    voltage = ads.readADC_SingleEnded(0) / 10;
    temperature = readTemperature();
    char ENTERPH[8] = "ENTERPH";
    ph.calibration(voltage, temperature, ENTERPH);
  }
  void exit_ph_calibration_mode() {
    voltage = ads.readADC_SingleEnded(0) / 10;
    temperature = readTemperature();
    char EXITPH[7] = "EXITPH";
    ph.calibration(voltage, temperature, EXITPH);
  }
  void calibrate_ph() {
    voltage = ads.readADC_SingleEnded(0) / 10;
    temperature = readTemperature();
    char CALPH[6] = "CALPH";
    ph.calibration(voltage, temperature, CALPH);
  }
};
