#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_MAG3110.h>  // magnometer library
#include <Adafruit_MMA8653.h>  // accelerometer library
#include <Adafruit_Microbit.h> // base microbit library
#include <Adafruit_Sensor.h>   // adafruit sensor helper

#include "Logger.h" // custome logger

Adafruit_Microbit uBit;
Adafruit_MMA8653 accel;
Adafruit_MAG3110 mag;

void setup() {
  // initialize the logger & serial port
  setupLogger();

  LOGGER(INFO, "Initializing uBit");

  Wire.begin(); // initialize I2C for sensors
  uBit.begin(); // initialize micrbot object

  if (!accel.begin()) {
    LOGGER(ERROR, "MMA8653 not found on I2C (check wiring/version)");
  } else {
    accel.setRange(MMA8653_RANGE_2_G); // 2/4/8g
    LOGGER(INFO, "MMA8653 ready");
  }
}

void loop() {}
