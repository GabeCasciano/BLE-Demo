#ifndef DSTRUCT_H_
#define DSTRUCT_H_

#include <Arduino.h>

/**
 * @brief Data structure for the sensor data
 */
struct SensorData_t {
  int32_t xcl_data[3], mag_data[3];

  /*
   * @brief Defult Constructor
   *
   * Sets all values to 0
   */
  SensorData_t() {
    memset((void *)xcl_data, 0, sizeof(int32_t) * 3);
    memset((void *)mag_data, 0, sizeof(int32_t) * 3);
  }

  /**
   * @brief Destructor
   */
  ~SensorData_t() = default;
};

/**
 * @brief Data structure for the button data
 */
struct ButtonData_t {
  uint8_t A, B;

  /**
   * @brief Default Constructor
   */
  ButtonData_t() = default;

  /**
   * @brief Paramaterized Constructor
   *
   * @param A A button value
   * @param B B button value
   */
  ButtonData_t(uint8_t A, uint8_t B) : A(A), B(B) {}

  /**
   * @brief Destructor
   */
  ~ButtonData_t() = default;
};

#endif
