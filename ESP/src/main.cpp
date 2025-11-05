#include <Arduino.h>

#include "BLE.hpp"
#include "Board.h"
#include "Logger.h"

void setup() {
  setupLogger();
  setupBoard();
  setupBle();
}

void loop() {
  LOGGER(INFO, "Loop");
  vTaskDelay(10);
}
