#ifndef BLE_H_
#define BLE_H_

#include <Arduino.h>
#include <NimBLEDevice.h>
#include <string>

#include "Logger.h"
#include "NimBLEAdvertising.h"
#include "NimBLECharacteristic.h"
#include "nimble/nimble/host/include/host/ble_hs.h"

#ifndef BLE_NAME
#define BLE_NAME "gabes-ble-demo"
#endif

#ifndef SERVICE_NAME
#define SERVICE_NAME "gabes-board-service"
#endif

class ServerCallbacks : public NimBLEServerCallbacks {
  void onConnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo) {
    LOGGER(INFO, "Client address: %s\n",
           connInfo.getAddress().toString().c_str());
  }

  void onDisconnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo,
                    int reason) {
    LOGGER(INFO, "Client disconnected - start advertising\n");
    NimBLEDevice::startAdvertising();
  }
};

class CharacteristicCallbacks : public NimBLECharacteristicCallbacks {
public:
  void onSubscribe(NimBLECharacteristic *pCharacteristic,
                   NimBLEConnInfo &connInfo, uint16_t subValue) {
    LOGGER(INFO, "%s : onSubscribe(), subValue: %d",
           pCharacteristic->getUUID().toString().c_str(), subValue);
  }
};

class PhotoCharacteristicCallbacks : public CharacteristicCallbacks {
  void onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) {
    LOGGER(INFO, "%s : PHOTO:onRead(), value: %s\n",
           pCharacteristic->getUUID().toString().c_str(),
           pCharacteristic->getValue().c_str());
  }
};

class LedCharacteristicCallback : public CharacteristicCallbacks {
  void onWrite(NimBLECharacteristic *pCharacteristic,
               NimBLEConnInfo &connInfo) {
    LOGGER(INFO, "%s : LED:onWrite(), value: %s\n",
           pCharacteristic->getUUID().toString().c_str(),
           pCharacteristic->getValue().c_str());
  }
};

class AinCharacteristicCallback : public CharacteristicCallbacks {
  void onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) {
    LOGGER(INFO, "%s : AIN:onRead(), value: %s\n",
           pCharacteristic->getUUID().toString().c_str(),
           pCharacteristic->getValue().c_str());
  }
};

class ButtonCharacteristicCallback : public CharacteristicCallbacks {
  void onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) {
    LOGGER(INFO, "%s : BUTTON:onRead(), value: %s\n",
           pCharacteristic->getUUID().toString().c_str(),
           pCharacteristic->getValue().c_str());
  }
};

/** Handler class for descriptor actions */
class DescriptorCallbacks : public NimBLEDescriptorCallbacks {
  void onWrite(NimBLEDescriptor *pDescriptor, NimBLEConnInfo &connInfo) {
    String dscVal = pDescriptor->getValue();
    LOGGER(INFO, "Descriptor written value: %s\n", dscVal.c_str());
  }

  void onRead(NimBLEDescriptor *pDescriptor, NimBLEConnInfo &connInfo) {
    LOGGER(INFO, "%s Descriptor read\n",
           pDescriptor->getUUID().toString().c_str());
  }
};

#ifndef UUID_SERVICE
#define UUID_SERVICE "dbbb06b1-1b2a-4a42-ac4e-6c64a9d88960"
#endif

#ifndef UUID_PHOTO_CHAR
#define UUID_PHOTO_CHAR "dbbb06b1-1b2a-4a42-ac4e-6c64a9d88961"
#endif

#ifndef UUID_LED_CHAR
#define UUID_LED_CHAR "dbbb06b1-1b2a-4a42-ac4e-6c64a9d88962"
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

static ServerCallbacks server_callbacks;

static AinCharacteristicCallback ainCharCallbacks;
static LedCharacteristicCallback ledCharCallbacks;
static PhotoCharacteristicCallbacks photoCharCallbacks;
static ButtonCharacteristicCallback btnCharCallbacks;

// My MAC: 0C:8B:95:34:6F:2E from bluetoothctl

void setupBle() {

  std::string ble_name(BLE_NAME);

  LOGGER(INFO, "Initializing BLE\n Device name: %s", ble_name.c_str());

  NimBLEDevice::init(ble_name);
  NimBLEDevice::setDeviceName(BLE_NAME);

  NimBLEDevice::setSecurityAuth(true, false, true);

  ble_server = NimBLEDevice::createServer();
  ble_server->setCallbacks(&server_callbacks);

  ble_board_service = ble_server->createService(UUID_SERVICE);

  ble_ain_char = ble_board_service->createCharacteristic(
      UUID_AIN_CHAR, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
  ble_ain_char->setCallbacks(&ainCharCallbacks);
  ble_ain_char->setValue(0);

  ble_btn_char = ble_board_service->createCharacteristic(
      UUID_BTN_CHAR, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
  ble_btn_char->setCallbacks(&btnCharCallbacks);
  ble_btn_char->setValue(0);

  ble_led_char = ble_board_service->createCharacteristic(
      UUID_LED_CHAR, NIMBLE_PROPERTY::WRITE_ENC);
  ble_led_char->setCallbacks(&ledCharCallbacks);

  ble_photo_char = ble_board_service->createCharacteristic(
      UUID_PHOTO_CHAR, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
  ble_photo_char->setCallbacks(&photoCharCallbacks);
  ble_photo_char->setValue(0);

  ble_board_service->start();

  ble_advertising = NimBLEDevice::getAdvertising();
  ble_advertising->setName("gabes-ad");
  ble_advertising->addServiceUUID(ble_board_service->getUUID());
  ble_advertising->setScanResponse(true);

  ble_advertising->start();
}

void updateButtonChar(bool button_state) {}

#endif
