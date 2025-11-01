#ifndef DSTRUCT_H_
#define DSTRUCT_H_

#include <Arduino.h>

struct Vec3_t {
  int x, y, z;

  Vec3_t() = default;
  Vec3_t(int x, int y, int z) : x(x), y(y), z(z) {}
  ~Vec3_t() = default;
};

struct SensorData_t {
  Vec3_t accel, mag;
  SensorData_t() = default;
  SensorData_t(Vec3_t accel, Vec3_t mag) : accel(accel), mag(mag) {}
  ~SensorData_t() = default;
};

struct ButtonData_t {
  uint8_t A, B;

  ButtonData_t() = default;
  ButtonData_t(uint8_t A, uint8_t B) : A(A), B(B) {}
  ~ButtonData_t() = default;
}

#endif
