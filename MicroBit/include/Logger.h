#ifndef LOGGER_H_
#define LOGGER_H_

#include "nrf51.h"
#include <Arduino.h>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

enum LOG_LEVEL { INFO, WARNING, ERROR };

inline const char *logString(LOG_LEVEL level) {
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

inline void setupLogger() {
#ifdef DEBUG
  Serial.begin(115200);

  while (!Serial)
    delay(10);

  Serial.println("Connected");
#endif
}

inline void LOGGER(LOG_LEVEL level, const char *fmt, ...) {
#ifdef DEBUG

  constexpr size_t BUF_SIZE = 1024;
  char buffer[BUF_SIZE];

  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, BUF_SIZE, fmt, args);
  va_end(args);

  Serial.print(logString(level));
  Serial.println(buffer);
  Serial.flush();
  delay(250);

#endif
  if (level == ERROR) {
    NVIC_SystemReset();
  }
}

#endif
