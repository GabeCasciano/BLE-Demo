#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_Microbit.h> // base microbit library

#include "Logger.h"  // custome logger
#include "Sensors.h" // custome sensors helpep

Adafruit_Microbit uBit; // uBit object for the display

void setup() {
  // initialize the logger & serial port
  setupLogger();

  LOGGER(INFO, "Initializing uBit");

  uBit.begin(); // initialize micrbot object

  setupSensors();
}

void loop() { pollSensors(); }
