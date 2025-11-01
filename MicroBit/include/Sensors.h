#ifndef SENSORS_H_
#define SENSORS_H_

#include "MMA8653.h"
#include <SparkFun_MAG3110.h>

#include "DStruct.h" // custom data structures
#include "Logger.h"  // custome logger
#include "wiring_constants.h"
#include "wiring_digital.h"

MAG3110 mag = MAG3110();
MMA8653 xcl;

#define BUTTON_A (5)
#define BUTTON_B (11)

void setupSensors() {
  if (!mag.initialize()) {
    LOGGER(ERROR, "Could not initialize the magnometer");
  }

  LOGGER(INFO,
         "Starting and calibrating the magnometer, put the microbit down.");
  mag.start();
  mag.calibrate();

  while (mag.isCalibrating()) {
    delay(10);
    Serial.print(".");
  }

  LOGGER(INFO, "Done calibrating");
  xcl.begin(false, 2);
}

void setupButtons() {
  LOGGER(INFO, "Setting up buttons A: %d B: %d", BUTTON_A, BUTTON_B);
  pinMode(BUTTON_A, INPUT);
  pinMode(BUTTON_B, INPUT);
}

void readButtons(ButtonData_t *data) {
  data->A = digitalRead(BUTTON_A);
  data->B = digitalRead(BUTTON_B);
}

bool pollSensors(SensorData_t *data) {
  if (mag.isCalibrating())
    return false;

  if (mag.dataReady())
    mag.readMag(&data->mag.x, &data->mag.y, &data->mag.z);

  xcl.update();

  data->accel.x = xcl.getXG();
  data->accel.y = xcl.getYG();
  data->accel.z = xcl.getZG();

  return true;
}

#endif
