#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_Microbit.h> // base microbit library
#include <SparkFun_MAG3110.h>

#include "MMA8653.h"

#include "Logger.h" // custome logger

Adafruit_Microbit uBit;

void setup() {
  // initialize the logger & serial port
  setupLogger();

  LOGGER(INFO, "Initializing uBit");

  Wire.begin(); // initialize I2C for sensors
  uBit.begin(); // initialize micrbot object
}

void loop() {}
