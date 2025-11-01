#ifndef SENSORS_H_
#define SENSORS_H_

#include "MMA8653.h"
#include <SparkFun_MAG3110.h>

#include "DStruct.h" // custom data structures
#include "Logger.h"  // custome logger
#include "wiring_constants.h"
#include "wiring_digital.h"

#define BUTTON_A (5)
#define BUTTON_B (11)

MAG3110 mag = MAG3110();
MMA8653 xcl;

/**
 * @brief Initialize and setup the sensors
 */
void setupSensors() {
  // initialize the magnometer, if it failes error out
  if (!mag.initialize()) {
    LOGGER(ERROR, "Could not initialize the magnometer");
  }

  LOGGER(INFO,
         "Starting and calibrating the magnometer, put the microbit down.");
  mag.start();
  mag.calibrate();

  // wait for calibration to finish
  while (mag.isCalibrating()) {
    delay(10);
    Serial.print(".");
  }

  LOGGER(INFO, "Done calibrating");

  LOGGER(INFO, "Initializing accelerometer");
  xcl.begin(false, 2);

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
 * @param data reference to an instance of SensorData_t where data will be
 * stored
 *
 * @return false if sensors could not be read
 */
bool pollSensors(SensorData_t *data) {
  // if magnometer is calibration return false
  if (mag.isCalibrating() || !mag.dataReady())
    return false;

  // read the magnometer values
  mag.readMag(&data->mag.x, &data->mag.y, &data->mag.z);

  // poll the accelerometer
  xcl.update();

  // read the accelerometer values
  data->xcl.x = xcl.getXG();
  data->xcl.y = xcl.getYG();
  data->xcl.z = xcl.getZG();

  return true;
}

#endif
