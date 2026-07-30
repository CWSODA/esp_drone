#include "imu.hpp"
#include "imu/accelerometer.hpp"
#include "imu/gyroscope.hpp"
#include "imu/magnetometer.hpp"

#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <math.h>

// Magnetometer: QMC5883L, (10, 50, 100, or 200 Hz)
// Gyroscope: ITG3205, (1 or 8 kHz)
// Accelerometer: ADXL345, (800 Hz max)

constexpr float MAHONY_MAG_WEIGHT = 0.1f;
constexpr float MAHONY_P = 1.0f;
constexpr float MAHONY_I = 1.0f;

constexpr int32_t GYRO_FREQ = 8e3;
constexpr int32_t GYRO_CD_US = 1e6 / GYRO_FREQ;

constexpr int32_t MAG_FREQ = 60;
constexpr int32_t MAG_CD_US = 1e6 / MAG_FREQ;

constexpr int32_t ACCEL_FREQ = 60;
constexpr int32_t ACCEL_CD_US = 1e6 / ACCEL_FREQ;

constexpr int32_t DEBUG_FREQ = 10;
constexpr int32_t DEBUG_CD_US = 1e6 / DEBUG_FREQ;

void sensor_manager(void* pvParameters) {
    IMU imu;
    imu.init_mag();
    imu.init_accel();

    imu.read_mag();
    imu.read_accel();
    imu.init_gyro();  // init gyro last since it needs accel/mag data

    // create timers
    Timer gyro_timer(GYRO_CD_US);
    Timer mag_timer(MAG_CD_US);
    Timer accel_timer(ACCEL_CD_US);

    Timer debug_timer(DEBUG_CD_US);

    // sensor loop
    while (true) {
        // can optimize by getting time once and passing to each function
        int64_t delta_time_us;
        if (gyro_timer.should_run(delta_time_us)) {
            // imu.read_gyro();
            // imu.update_gyro(delta_time_us);
        }
        if (mag_timer.should_run(delta_time_us)) {
            imu.read_mag();
            imu.update_mag_err();
        }
        if (accel_timer.should_run(delta_time_us)) {
            imu.read_accel();
            imu.update_accel_err();
        }
        if (debug_timer.should_run(delta_time_us)) {
            Quat::from_data(imu.get_accel(), imu.get_mag()).disp('q');
            imu.get_accel().disp('a');
            imu.get_mag().disp('m');
        }
        taskYIELD();
    }
}

void IMU::update_accel_err() {
    Vec3 down = accel_data.normalize();

    // predicted orientation
    Vec3 p_down =
        (quat * Quat(0, 0, -1) * quat.conjugate()).to_vec().normalize();

    accel_err = p_down.cross(down);  // check order later

    update_error();
}

void IMU::update_mag_err() {
    Vec3 north = mag_data.normalize();

    // predicted orientation
    Vec3 p_north =
        (quat * Quat(1, 0, 0) * quat.conjugate()).to_vec().normalize();

    mag_err = p_north.cross(north);  // check order later

    update_error();
}

// combine magnetometer and accelerometer error
void IMU::update_error() {
    Vec3 err = accel_err + (mag_err * MAHONY_MAG_WEIGHT);
    total_err = err * MAHONY_P;
}

Vec3 IMU::calc_alt() {
    // pitch with accelerometer
    float m = sqrtf(accel_data.y * accel_data.y + accel_data.z * accel_data.z);
    float pitch = atan2(accel_data.x, m);
    float roll = atan2(accel_data.y, accel_data.z);

    float yaw = atan2(mag_data.x, mag_data.y);

    return Vec3(pitch, roll, yaw);
}