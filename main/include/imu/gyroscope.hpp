#include "imu.hpp"

constexpr float DEG2RAD = 3.1415926535897932384f / 180.0f;

// must init last?
void IMU::init_gyro() {
    gyroscope.init(GYRO_ADDR);

    gyroscope.write_reg(21, 0b0);           // no sample rate divider
    gyroscope.write_reg(22, 0b000'11'000);  // enable measurement
    gyroscope.write_reg(62, 0b00000'001);   // change clock source

    quat = Quat::from_data(accel_data, mag_data);
}

void IMU::read_gyro() {
    uint8_t data_read[6];
    gyroscope.read_6(data_read, 0x1D);

    for (int x = 0; x < 3; x++) {
        uint8_t MSB = data_read[x * 2];
        uint8_t LSB = data_read[x * 2 + 1];
        int16_t bit_val = (MSB << 8) + LSB;

        gyro_data.at(x) = bit_val * (1.0f / 14.375f);  // in °/s
    }
#if DEBUG_GYRO
    gyro_data.disp("Gyro");
#endif
}

void IMU::update_gyro(const int64_t delta_time_us) {
    // first few measurements are used for calibration
    static Vec3 offset;
    static int count = 0;
    if (count++ < 10) {
        offset += gyro_data * 0.1f;  // average the 10 samples
        return;
    }
    gyro_data -= offset + total_err;  // remove offset and PI error
    // gyro_data -= offset;  // remove offset and PI error

    float delta_time_s = delta_time_us * 1e-6;

    Quat dw = {0, gyro_data.x, gyro_data.y, gyro_data.z};
    Quat dq = (dw * quat) * 0.5f;
    Quat delta_q = dq * delta_time_s * DEG2RAD;
    quat = (quat + delta_q).normalize();
}

// should be device addr of 0x68
void IMU::check_gyro() {
    // register for identification register
    uint8_t buf = 0x00;
    gyroscope.write(&buf, 1);

    uint8_t byte;
    gyroscope.read(&byte, 1);
    printf("Gyro Identified Byte (expect 0x68): %02X\n", byte);
}