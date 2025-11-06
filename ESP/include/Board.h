#ifndef BOARD_H_
#define BOARD_H_

#include <Arduino.h>
#include <cstdint>

#include "Logger.h"
#include "esp32-hal-adc.h"
#include "esp32-hal-ledc.h"

#define BUTTON_PIN (25)
#define LED_PIN (17)
#define PHOTO_PIN (4)
#define AIN (2)

#define LED_FREQ (5000)
#define LED_RES (8)
#define LED_CHANNEL (0)

#define AIN_RES (12)

inline void setupBoard() {
  LOGGER(INFO, "Initializing board");

  pinMode(BUTTON_PIN, INPUT);

  LOGGER(INFO, "Button: %d\nAIN: %d", BUTTON_PIN, AIN);

  ledcSetup(LED_CHANNEL, LED_FREQ, LED_RES);
  ledcAttachPin(LED_PIN, LED_CHANNEL);
  ledcWrite(LED_CHANNEL, 0);

  analogReadResolution(AIN_RES);

  LOGGER(INFO,
         "LED Config:\n Pin: %d\n Channel: %d\n Frequency: %d\n Resolution: %d",
         LED_PIN, LED_CHANNEL, LED_FREQ, LED_RES);

  LOGGER(INFO, "Done ");
}

inline void setLedPWM(uint8_t duty_cycle) {
  ledcWrite(LED_CHANNEL, duty_cycle);
}

inline uint16_t readOversampleAnalog(uint8_t pin, uint8_t over_sample) {
  // make sure always greater than 0
  over_sample = (over_sample) ? over_sample : 1;

  uint32_t sum = 0;

  for (short i = 0; i < over_sample; ++i)
    sum += analogRead(PHOTO_PIN);
  return (uint16_t)(sum / over_sample);
}

#endif
