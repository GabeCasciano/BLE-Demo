#ifndef DSTRUCT_H_
#define DSTRUCT_H_

#include <Arduino.h>

/**
 * @brief A simple 3D vector structure.
 */
struct Vec3_t {
  int x, y, z;

  /**
   * @brief Default constructor
   */
  Vec3_t() = default;

  /**
   * @brief Paramaterized Constructor
   *
   * @param x x-axis value
   * @param y y-axis value
   * @param z z-axis value
   */
  Vec3_t(int x, int y, int z) : x(x), y(y), z(z) {}

  /**
   * @brief Destructor
   */
  ~Vec3_t() = default;
};

/**
 * @brief Data structure for the sensor data
 */
struct SensorData_t {
  Vec3_t xcl, mag;

  /*
   * @brief Defult Constructor
   */
  SensorData_t() = default;

  /**
   * @brief Paramaterized constructor
   *
   * @param xel accelerometer vector
   * @param mag magnometer vector
   */
  SensorData_t(Vec3_t xcl, Vec3_t mag) : xcl(xcl), mag(mag) {}

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
