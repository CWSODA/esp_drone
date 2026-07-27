#include "imu.hpp"

constexpr float MAGNO_RANGE = 2.0f;

void IMU::init_mag() {
    magnetometer.init(MAG_ADDR);

    // OSR = 512, 2G range, ODR = 200Hz, continuous mode
    magnetometer.write_reg(0x09, 0b00'00'11'01);
    magnetometer.write_reg(0x0A, 1 << 6);  // enable address pointer rollover
    magnetometer.write_reg(0x0B, 0x01);    // set period
}

void IMU::read_mag() {
    uint8_t data_read[6];
    magnetometer.read_6(data_read, 0x00);

    for (int x = 0; x < 3; x++) {
        uint8_t LSB = data_read[x * 2];
        uint8_t MSB = data_read[x * 2 + 1];
        int16_t bit_val = (MSB << 8) + LSB;
        mag_data.at(x) = bit_val * (MAGNO_RANGE / 32'767);  // range of 2 Gauss
    }
#if DEBUG_MAG
    mag_data.disp("Mag");
#endif
}

void IMU::check_mag() {
    // register for identification register
    uint8_t buf = 0x0D;
    magnetometer.write(&buf, 1);

    uint8_t byte;
    magnetometer.read(&byte, 1);
    printf("Mag Identified Byte (expect 0xFF): %02X\n", byte);
}