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

  uint8_t led_value;

  updateButtonChar(digitalRead(BUTTON_PIN));
  updatePhotoChar(readOversampleAnalog(PHOTO_PIN, 10));
  updateAinChar(readOversampleAnalog(AIN, 10));

  readLedChar(&led_value);
  setLedPWM(led_value);
}
