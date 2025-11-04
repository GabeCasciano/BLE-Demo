#ifndef BLE_H_
#define BLE_H_

#include <Arduino.h>
#include <NimBLEDevice.h>
#include <string>

#include "Logger.h"

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

    /**
     *  We can use the connection handle here to ask for different connection
     * parameters. Args: connection handle, min connection interval, max
     * connection interval latency, supervision timeout. Units; Min/Max
     * Intervals: 1.25 millisecond increments. Latency: number of intervals
     * allowed to skip. Timeout: 10 millisecond increments.
     */
    pServer->updateConnParams(connInfo.getConnHandle(), 24, 48, 0, 180);
  }

  void onDisconnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo,
                    int reason) {
    LOGGER(INFO, "Client disconnected - start advertising\n");
    NimBLEDevice::startAdvertising();
  }

  void onMTUChange(uint16_t MTU, NimBLEConnInfo &connInfo) {
    LOGGER(INFO, "MTU updated: %u for connection ID: %u\n", MTU,
           connInfo.getConnHandle());
  }

  /********************* Security handled here *********************/
  uint32_t onPassKeyDisplay() {
    LOGGER(INFO, "Server Passkey Display\n");
    /**
     * This should return a random 6 digit number for security
     *  or make your own static passkey as done here.
     */
    return 123456;
  }

  void onConfirmPassKey(NimBLEConnInfo &connInfo, uint32_t pass_key) {
    LOGGER(INFO, "The passkey YES/NO number: %" PRIu32 "\n", pass_key);
    /** Inject false if passkeys don't match. */
    onConfirmPassKey(connInfo, true);
  }

  void onAuthenticationComplete(NimBLEConnInfo &connInfo) {
    /** Check that encryption was successful, if not we disconnect the client */
    if (!connInfo.isEncrypted()) {
      NimBLEDevice::getServer()->disconnect(connInfo.getConnHandle());
      LOGGER(INFO, "Encrypt connection failed - disconnecting client\n");
      return;
    }

    LOGGER(INFO, "Secured connection to: %s\n",
           connInfo.getAddress().toString().c_str());
  }
};

class CharacteristicCallbacks : public NimBLECharacteristicCallbacks {
public:
  void onSubscribe(NimBLECharacteristic *pCharacteristic,
                   NimBLEConnInfo &connInfo, uint16_t subValue) {
    // String str = "Client ID: ";
    // str += connInfo.getConnHandle();
    // str += " Address: ";
    // str += connInfo.getAddress().toString();
    // if (subValue == 0) {
    //   str += " Unsubscribed to ";
    // } else if (subValue == 1) {
    //   str += " Subscribed to notifications for ";
    // } else if (subValue == 2) {
    //   str += " Subscribed to indications for ";
    // } else if (subValue == 3) {
    //   str += " Subscribed to notifications and indications for ";
    // }
    // str += std::string(pCharacteristic->getUUID());
    //
    // LOGGER(INFO, "%s\n", str.c_str());
  }
};

class PhotoCharacteristicCallbacks : public CharacteristicCallbacks {
  void onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) {
    LOGGER(INFO, "%s : PHOTO:onRead(), value: %s\n",
           pCharacteristic->getUUID().toString().c_str(),
           pCharacteristic->getValue().c_str());
  }
};

class DoutCharacteristicCallbacks : public CharacteristicCallbacks {
  void onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) {
    LOGGER(INFO, "%s : DIN:onRead(), value: %s\n",
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
  void onWrite(NimBLECharacteristic *pCharacteristic,
               NimBLEConnInfo &connInfo) {
    LOGGER(INFO, "%s : AIN:onWrite(), value: %s\n",
           pCharacteristic->getUUID().toString().c_str(),
           pCharacteristic->getValue().c_str());
  }
};

class DinCharacteristicCallback : public CharacteristicCallbacks {
  void onWrite(NimBLECharacteristic *pCharacteristic,
               NimBLEConnInfo &connInfo) {
    LOGGER(INFO, "%s : DIN:onWrite(), value: %s\n",
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

static NimBLEServer *ble_server;
static NimBLEService *ble_board_service;
static NimBLECharacteristic *ble_photo_char, *ble_led_char, *ble_ain_chat,
    *ble_din_char, *ble_dout_char;

static ServerCallbacks server_callbacks;
void setupBle() {

  std::string ble_name(BLE_NAME);

  LOGGER(INFO, "Initializing BLE\n Device name: %s", ble_name.c_str());
  NimBLEDevice::init(ble_name);

  ble_server = NimBLEDevice::createServer();
  ble_server->setCallbacks(&server_callbacks);
}

#endif
