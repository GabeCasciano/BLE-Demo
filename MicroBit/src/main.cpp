#include <Arduino.h>
#include <Wire.h>
#include <cstdint>
#include <time.h>

#include <Adafruit_Microbit.h> // base microbit library

#include "BLE.h" // custom ble helper
#include "DStruct.h"
#include "Logger.h"  // custome logger
#include "Sensors.h" // custome sensors helper
#include "delay.h"

#ifndef LOOP_RATE
#define LOOP_RATE (100)
#endif

// offset from the loop timer to wake up
#ifndef LOOP_OFFSET
#define LOOP_OFFSET (5)
#endif

constexpr time_t PERIOD_US =
    1000000UL / LOOP_RATE; // calculate the sleep interval

Adafruit_Microbit uBit; // uBit object for the display

// Data structures for passing between funcs and coms
SensorData_t sensorData;
ButtonData_t btnData;

void setup() {
  // initialize the logger & serial port
  setupLogger();

  LOGGER(INFO, "Initializing uBit ....");
  uBit.begin();

  LOGGER(INFO, "Initializing sensors");
  setupSensors();

  LOGGER(INFO, "Initializing BLE");
  setupBLE();
}

// application code to run in the loop at a fixed timing
void app() {
  // if pollAndConnect returns false, early return
  if (!pollAndConnect())
    return;

  // if we are connected, read the matrix num and display it
  uBit.matrix.print(readMatrixNum());

  // if data is availble from the sensors - capture it - otherwise return early
  if (!pollSensors(&sensorData))
    return;

  // advertise collected data
  advertiseSensorChar(sensorData);

  // read buttons
  readButtons(&btnData);

  // advertise data
  advertiseButtonChar(btnData);
}

void loop() {
  // get the current timestamp
  time_t next_ts = micros();

  // run the application logic
  app();

  // calculate the amount of time to wait
  int32_t time_to_wait = (int32_t)((next_ts + PERIOD_US) - micros());

  // if the time to wait is greater than the loop offset - then wait
  if (time_to_wait > LOOP_OFFSET) {
    delayMicroseconds(time_to_wait - LOOP_OFFSET);
  }
}
