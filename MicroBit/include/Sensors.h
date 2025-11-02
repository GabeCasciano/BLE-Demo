#ifndef SENSORS_H_
#define SENSORS_H_

#include <LSM303AGR_ACC_Sensor.h>
#include <LSM303AGR_MAG_Sensor.h>

#include "DStruct.h" // custom data structures
#include "Logger.h"  // custome logger
#include "wiring_constants.h"
#include "wiring_digital.h"

#define BUTTON_A (5)
#define BUTTON_B (11)

LSM303AGR_ACC_Sensor mag(&Wire);
LSM303AGR_ACC_Sensor xcl(&Wire);

/**
 * @brief Initialize and setup the sensors
 */
void setupSensors() {

  // init i2c
  LOGGER(INFO, "Initializing I2C");
  Wire.begin();

  LOGGER(INFO, "Initializing Mag");
  mag.begin();
  mag.Enable();

  LOGGER(INFO, "Initializing Xcl");
  xcl.begin();
  xcl.Enable();

  LOGGER(INFO, "Done Sensors");
}

/**
 * @brief Initialize and setup buttons
 */
void setupButtons() {
  LOGGER(INFO, "Setting up buttons A: %d B: %d", BUTTON_A, BUTTON_B);
  pinMode(BUTTON_A, INPUT);
  pinMode(BUTTON_B, INPUT);
  LOGGER(INFO, "Done Buttons");
}

/**
 * @brief read the state of the buttons into the provied data strcuture
 *
 * @param data reference to an instance of ButtonData_t where data will be
 * stored
 */
void readButtons(ButtonData_t *data) {
  data->A = digitalRead(BUTTON_A);
  data->B = digitalRead(BUTTON_B);
}

/**
 * @brief poll the sensors and read them if possible
 *
 * @param xcl_data reference to an int32_t[3]
 * @param mag_data reference to an int32_t[3]
 *
 */
void pollSensors(SensorData_t *data) {
  xcl.GetAxes(data->xcl_data);
  mag.GetAxes(data->mag_data);
}

#endif
