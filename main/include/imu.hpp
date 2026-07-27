#pragma once

#include "esp_timer.h"

#include "i2c.hpp"
#include "vec3.hpp"
#include "quaternion.hpp"

// Magnetometer: QMC5883L, (10, 50, 100, or 200 Hz)
// Gyroscope: ITG3205, (1 or 8 kHz)
// Accelerometer: ADXL345, (800 Hz max)
constexpr uint16_t GYRO_ADDR = 0b1101000;
constexpr uint16_t ACCEL_ADDR = 0x53;  // 0x1D or 0x53
constexpr uint16_t MAG_ADDR = 0x0D;

#define DEBUG_ACCEL false
#define DEBUG_MAG false
#define DEBUG_GYRO false

class Timer {
   public:
    Timer(int32_t cd_us) { cooldown = cd_us; }

    // checks whether the cooldown has expired
    // returns the delta time
    bool should_run(int64_t& delta_time_us) {
        int64_t current_time = esp_timer_get_time();

        if (current_time < tgt_time) return false;

        delta_time_us = current_time - prev_time;
        tgt_time = current_time + cooldown;
        prev_time = current_time;
        return true;
    }

    int64_t cooldown;

   private:
    int64_t tgt_time = 0;
    int64_t prev_time = 0;
};

class IMU {
   public:
    void init_gyro();
    void read_gyro();
    void update_gyro(const int64_t delta_time_us);
    void check_gyro();  // should be device addr

    void init_mag();
    void read_mag();
    void update_mag_err();
    void check_mag();

    void init_accel();
    void read_accel();
    void update_accel_err();
    void check_accel();

    void update_error();
    Vec3 calc_alt();

    const Vec3& get_accel() const { return accel_data; }
    const Vec3& get_mag() const { return mag_data; }

    const Quat& get_quat() const { return quat; }

   private:
    I2CDevice accelerometer;
    I2CDevice gyroscope;
    I2CDevice magnetometer;

    // accelerometer measures reactant force which is opposite of gravity!
    Vec3 accel_data = Vec3::zeros();
    Vec3 gyro_data = Vec3::zeros();
    Vec3 mag_data = Vec3::zeros();

    Quat quat;
    Vec3 accel_err = Vec3::zeros();
    Vec3 mag_err = Vec3::zeros();
    Vec3 total_err = Vec3::zeros();
};

// task for handling sensor reading and data processing
void sensor_manager(void* pvParameters);

inline int64_t time() {
    static int64_t prev;

    int64_t current_time = esp_timer_get_time();
    int64_t diff = current_time - prev;
    prev = current_time;

    return diff;
}