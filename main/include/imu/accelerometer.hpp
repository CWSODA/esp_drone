#include "imu.hpp"

void IMU::init_accel() {
    accelerometer.init(ACCEL_ADDR);                // or 0x1D
    accelerometer.write_reg(0x31, 0b0000'1'0'01);  // ±4g range
    accelerometer.write_reg(0x2C, 0b0000'1100);    // 400 Hz
    accelerometer.write_reg(0x2D, 0b0000'1'000);   // start measuring
}
void IMU::read_accel() {
    uint8_t data_read[6];
    accelerometer.read_6(data_read, 0x32);

    for (int x = 0; x < 3; x++) {
        uint8_t LSB = data_read[x * 2];
        uint8_t MSB = data_read[x * 2 + 1];
        int16_t bit_val = (MSB << 8) + LSB;

        accel_data.at(x) = bit_val * 4e-3f;  // in g
    }
#if DEBUG_ACCEL
    accel_data.disp("Accel");
#endif
}
void IMU::check_accel() {
    // register for identification register
    uint8_t buf = 0x00;
    accelerometer.write(&buf, 1);

    uint8_t byte;
    accelerometer.read(&byte, 1);
    printf("Accel Identified Byte (expect 0xE5): %02X\n", byte);
}
