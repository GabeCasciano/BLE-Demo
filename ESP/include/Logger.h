#ifndef LOGGER_H_
#define LOGGER_H_

#include "esp_system.h"
#include <Arduino.h>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

/* *** IMPORTANT *** */
/*
 * LOGGER depends on the DEBUG flag, otherwise it will not be implemented
 */

/**
 * @brief Different log levels
 */
enum LOG_LEVEL { INFO, WARNING, ERROR };

/**
 * @brief Conevert a LOG_LEVEL enum into it's string representation
 *
 * @param level LOG_LEVEL to be converted
 *
 * @return corresponding log level as const char *
 */
const char *logString(LOG_LEVEL level) {
  switch (level) {
  case INFO:
    return "[INFO]";
  case WARNING:
    return "[WARNING]";
  case ERROR:
    return "[ERROR]";
  default:
    return "[UNKNOWN]";
  }
}

/**
 * @brief Output a log messaage to the serial printer
 *
 * Use this logger like printf() in C stdlib
 *
 * @param level log level for this log message
 * @param fmt format string
 * @param ... format parameters
 */

void LOGGER(LOG_LEVEL level, const char *fmt, ...) {
#ifdef DEBUG
  // create a char buffer
  constexpr size_t BUF_SIZE = 1024;
  char buffer[BUF_SIZE];

  // process arguments
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, BUF_SIZE, fmt, args);
  va_end(args);

  Serial.print(logString(level)); // print log level
  Serial.println(buffer);         // print log string
  Serial.flush(); // flush the serial port to make sure it writes
  delay(250);     // delay 250 ms to wait for flush

#endif

  // if the level is ERROR, restart the micro
  if (level == ERROR) {
    abort();
  }
}

/**
 * @brief Setup the serial printer and wait for it to connect
 */
void setupLogger() {
#ifdef DEBUG
  Serial.begin(115200);

  while (!Serial)
    delay(10);

  delay(1000);

  Serial.println("Connected");
#endif
}

#endif
