#ifndef DSTRUCT_H_
#define DSTRUCT_H_

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

#endif
