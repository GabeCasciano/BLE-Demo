#ifndef BOARD_H_
#define BOARD_H_

#include <Arduino.h>
#include <cstdint>

#include "Logger.h"
#include "esp32-hal-adc.h"
#include "esp32-hal-gpio.h"
#include "esp32-hal-ledc.h"

#define BUTTON_PIN (6)
#define LED_PIN (1)
#define PHOTO_PIN (0)
#define AIN (0)
#define DIN0 (2)
#define DIN1 (3)
#define DOUT0 (4)
#define DOUT1 (5)

#define LED_FREQ (5000)
#define LED_RES (8)
#define LED_CHANNEL (0)

#define AIN_RES (12)

void setupBoard() {
  LOGGER(INFO, "Initializing board");

  pinMode(BUTTON_PIN, INPUT);
  pinMode(DIN0, INPUT);
  pinMode(DIN1, INPUT);
  pinMode(DOUT0, OUTPUT);
  pinMode(DOUT1, OUTPUT);

  LOGGER(INFO, "Button: %d\nDIN: %d, %d\nDOUT: %d, %d\nAIN: %d)", BUTTON_PIN,
         DIN0, DIN1, DOUT0, DOUT1, AIN);

  ledcSetup(LED_CHANNEL, LED_FREQ, LED_RES);
  ledcAttachPin(LED_PIN, LED_CHANNEL);
  ledcWrite(LED_CHANNEL, 0);

  analogReadResolution(AIN_RES);

  LOGGER(INFO,
         "LED Config:\n Pin: %d\n Channel: %d\n Frequency: %d\n Resolution: %d",
         LED_PIN, LED_CHANNEL, LED_FREQ, LED_RES);

  LOGGER(INFO, "Done ");
}

void setLedPWM(uint8_t duty_cycle) { ledcWrite(LED_CHANNEL, duty_cycle); }

uint16_t readOversampleAnalog(uint8_t pin, uint8_t over_sample) {
  // make sure always greater than 0
  over_sample = (over_sample) ? over_sample : 1;

  uint32_t sum;

  for (short i = 0; i < over_sample; ++i)
    sum += analogRead(PHOTO_PIN);
  return (uint16_t)(sum / over_sample);
}

#endif
