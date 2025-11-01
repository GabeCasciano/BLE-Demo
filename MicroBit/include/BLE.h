#ifndef BLE_H_
#define BLE_H_

#include <Arduino.h>

#include "BLEAttribute.h"
#include <BLEPeripheral.h> // BLE library
#include <cstdint>

#include "DStruct.h"
#include "Logger.h"

#ifndef UUID_SERVICE
#define UUID_SERVICE "dbbb06b1-1b2a-4a42-ac4e-6c64a9d88960"
#endif

#ifndef UUID_SENSOR_CHAR
#define UUID_SENSOR_CHAR "dbbb06b1-1b2a-4a42-ac4e-6c64a9d88961"
#endif

#ifndef UUID_BUTTON_CHAR
#define UUID_BUTTON_CHAR "dbbb06b1-1b2a-4a42-ac4e-6c64a9d88962"
#endif

#ifndef UUID_MATRIX_CHAR
#define UUID_MATRIX_CHAR "dbbb06b1-1b2a-4a42-ac4e-6c64a9d88963"
#endif

#ifndef LOCAL_NAME
#define LOCAL_NAME "gabes-mbit-demo"
#endif

#ifndef DEV_NAME
#define DEV_NAME "gabes-mbit"
#endif

BLEPeripheral ble;                // wrapper for the peripheral
BLEService service(UUID_SERVICE); // ble service object

BLECharacteristic sensor_char(UUID_SENSOR_CHAR, BLERead | BLENotify,
                              sizeof(SensorData_t));
BLECharacteristic button_char(UUID_BUTTON_CHAR, BLERead | BLENotify,
                              sizeof(ButtonData_t));
BLECharacteristic matrix_char(UUID_MATRIX_CHAR,
                              BLEWrite | BLEWriteWithoutResponse, 1);

/**
 * @brief Initialize and setup BLE, services and characteristics
 */
void setupBLE() {

  LOGGER(INFO,
         "Settuping up BLE:\n dev name: %s\n local name: %s\n service: %s",
         DEV_NAME, LOCAL_NAME, service.uuid());

  ble.setDeviceName(DEV_NAME);
  ble.setLocalName(LOCAL_NAME);
  ble.setAdvertisedServiceUuid(service.uuid());

  ble.addAttribute(service);
  ble.addAttribute(sensor_char);
  ble.addAttribute(button_char);
  ble.addAttribute(matrix_char);

  ble.begin();
}

/**
 * @brief Polls and BLE perif and returns connection status
 *
 * @return true if connected
 */
bool pollAndConnect() {
  ble.poll();

  return ble.central().connected();
}

/**
 * @brief Advertises the provided sensor data on corresponding characteristic
 *
 * @param data sensor data to be advertised
 */
void advertiseSensorChar(SensorData_t data) {}

/**
 * @brief Advertises the provided button data on corresponding characteristic
 *
 * @param data button data to be advertised
 */
void advertiseButtonChar(ButtonData_t data) {}

/**
 * @brief Reads an uin8_t from Matrix characteristic
 *
 * @return value that is read, 0 if no value is read
 */
uint8_t readMatrixNum() {
  // Need to fix - Will always return 0 unless constantly getting a value
  // take a ptr  to the data and return a bool
  if (!matrix_char.written() && matrix_char.valueLength() < 1)
    return 0;

  return (uint8_t)matrix_char.value()[0];
}

#endif
