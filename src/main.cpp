#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_Microbit.h>

#include "Logger.h"

Adafruit_Microbit uBit;

void setup() {
  // initialize the logger & serial port
  setupLogger();

  LOGGER(INFO, "Initializing uBit");

  Wire.begin(); // initialize I2C for sensors
  uBit.begin(); // initialize micrbot object

  ubit.display().setBrightness(0);
}

void loop() {}
