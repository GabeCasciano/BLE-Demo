#ifndef BLE_H_
#define BLE_H_

#include <Arduino.h>

#include "BLEAttribute.h"
#include <BLEPeripheral.h> // BLE library

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

/* The different BLE characteristics that will be advertised
    - The data to transmit will notify the receiver when it has new data and
   allow it to read.
    - The data to read will be read by the microbit and can be check when we
   have time.
*/
BLECharacteristic sensor_char(UUID_SENSOR_CHAR, BLERead | BLENotify,
                              sizeof(SensorData_t));
BLECharacteristic button_char(UUID_BUTTON_CHAR, BLERead | BLENotify, 2);
BLECharacteristic matrix_char(UUID_MATRIX_CHAR,
                              BLEWrite | BLEWriteWithoutResponse, sizeof(char));

inline void setupBLE() {

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

inline bool pollAndConnect() {
  ble.poll();

  return ble.central().connected();
}

inline void advertiseSensorChar(SensorData_t data) {}

inline void advertiseButtonChar() {}

inline char readMatrixChar() {
  if (!matrix_char.written() && matrix_char.valueLength() < 1)
    return '\0';

  return (char)matrix_char.value()[0];
}

#endif
