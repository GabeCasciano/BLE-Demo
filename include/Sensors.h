#ifndef SENSORS_H_
#define SENSORS_H_

#include "MMA8653.h"
#include <SparkFun_MAG3110.h>

#include "DStruct.h" // custom data structures
#include "Logger.h"  // custome logger

SensorData_t data;

MAG3110 mag = MAG3110();
MMA8653 xcl;

inline void setupSensors() {
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

void pollSensors() {
  if (mag.isCalibrating())
    return;

  if (mag.dataReady())
    mag.readMag(&data.mag.x, &data.mag.y, &data.mag.z);

  xcl.update();
}

#endif
