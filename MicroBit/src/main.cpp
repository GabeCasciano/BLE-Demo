#include <Arduino.h>
#include <Wire.h>
#include <cstdint>
#include <time.h>

#include <Adafruit_Microbit.h> // base microbit library

#include "BLE.h"     // custom ble helper
#include "Logger.h"  // custome logger
#include "Sensors.h" // custome sensors helper
#include "delay.h"

Adafruit_Microbit uBit; // uBit object for the display

#ifndef LOOP_RATE
#define LOOP_RATE (1000)
#endif

#ifndef LOOP_OFFSET
#define LOOP_OFFSET (5)
#endif

constexpr time_t PERIOD_US = 1000000UL / LOOP_RATE;

void setup() {
  // initialize the logger & serial port
  setupLogger();

  LOGGER(INFO, "Initializing uBit");

  uBit.begin(); // initialize micrbot object

  setupSensors();
  setupBLE();
}

void iter() {
  pollSensors();
  if (pollAndConnect()) {
    auto mat = readMatrixChar();
  }
}

void loop() {

  time_t next_ts = micros();

  iter();

  next_ts += PERIOD_US;

  int32_t time_to_wait = (int32_t)(next_ts - micros());
  if (time_to_wait > LOOP_OFFSET) {
    delayMicroseconds(time_to_wait - LOOP_OFFSET);
  }
}
