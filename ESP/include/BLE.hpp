#ifndef BLE_H_
#define BLE_H_

#include <Arduino.h>
#include <stdint.h>
#include <string>

#include "Board.h"
#include "Logger.h"

#include "NimBLEAdvertising.h"
#include "NimBLECharacteristic.h"
#include <NimBLEDevice.h>

#ifndef BLE_NAME
#define BLE_NAME "gabes-ble-demo"
#endif

#ifndef SERVICE_NAME
#define SERVICE_NAME "gabes-board-service"
#endif

#ifndef UUID_SERVICE
#define UUID_SERVICE "dbbb06b1-1b2a-4a42-ac4e-6c64a9d88960"
#endif

#ifndef UUID_PHOTO_CHAR
#define UUID_PHOTO_CHAR "dbbb06b1-1b2a-4a42-ac4e-6c64a9d88961"
#endif

#ifndef UUID_LED_CHAR
#define UUID_LED_CHAR "dbbb06b1-1b2a-4a42-ac4e-6c64a9d88965"
#endif

#ifndef UUID_AIN_CHAR
#define UUID_AIN_CHAR "dbbb06b1-1b2a-4a42-ac4e-6c64a9d88963"
#endif

#ifndef UUID_BTN_CHAR
#define UUID_BTN_CHAR "dbbb06b1-1b2a-4a42-ac4e-6c64a9d88964"
#endif

static NimBLEServer *ble_server;
static NimBLEService *ble_board_service;
static NimBLEAdvertising *ble_advertising;
static NimBLECharacteristic *ble_photo_char, *ble_led_char, *ble_ain_char,
    *ble_btn_char;

// My MAC: 0C:8B:95:34:6F:2E from bluetoothctl

void setupBle() {

  std::string ble_name(BLE_NAME);

  LOGGER(INFO, "Initializing BLE\n Device name: %s", ble_name.c_str());

  NimBLEDevice::init(ble_name);
  NimBLEDevice::setDeviceName(BLE_NAME);

  NimBLEDevice::setSecurityAuth(true, false, true);

  ble_server = NimBLEDevice::createServer();

  ble_board_service = ble_server->createService(UUID_SERVICE);

  ble_ain_char = ble_board_service->createCharacteristic(
      UUID_AIN_CHAR, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY,
      sizeof(uint16_t));
  ble_ain_char->setValue(0);

  ble_btn_char = ble_board_service->createCharacteristic(
      UUID_BTN_CHAR, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY,
      sizeof(bool));
  ble_btn_char->setValue(0);

  ble_led_char = ble_board_service->createCharacteristic(
      UUID_LED_CHAR, NIMBLE_PROPERTY::WRITE_NR, sizeof(uint8_t));

  ble_photo_char = ble_board_service->createCharacteristic(
      UUID_PHOTO_CHAR, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY,
      sizeof(uint16_t));
  ble_photo_char->setValue(0);

  ble_board_service->start();

  ble_advertising = NimBLEDevice::getAdvertising();
  ble_advertising->setName("gabes-ad");
  ble_advertising->addServiceUUID(ble_board_service->getUUID());
  ble_advertising->setScanResponse(true);

  ble_advertising->start();
}

void updateButtonChar(bool button_state) {
  ble_btn_char->setValue(button_state);
  ble_btn_char->notify();
}

void updatePhotoChar(uint16_t reading) {
  ble_photo_char->setValue(reading);
  ble_photo_char->notify();
}

void updateAinChar(uint16_t reading) {
  ble_ain_char->setValue(reading);
  ble_ain_char->notify();
}

bool readLedChar(uint8_t *data) {
  std::string val = ble_led_char->getValue();
  if (val.empty())
    return false;

  *data = (uint8_t)val[0];

  return true;
}

#endif
