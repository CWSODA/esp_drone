#pragma once

#include "driver/i2c_master.h"

constexpr i2c_port_num_t I2C_PORT = 0;
constexpr gpio_num_t SDA_PIN = GPIO_NUM_21;
constexpr gpio_num_t SCL_PIN = GPIO_NUM_22;

// only run once unless more i2c ports are needed
void init_i2c_bus();

class I2CDevice {
   public:
    I2CDevice() {}
    I2CDevice(uint16_t addr);

    void init(uint16_t addr);

    void write(const uint8_t* buf, const size_t len) {
        int timeout = -1;  // timeout of -1 means wait forever
        i2c_master_transmit(device_handle_, buf, len, timeout);
    }
    void write_reg(const uint8_t addr, const uint8_t val) {
        uint8_t buf[] = {addr, val};
        i2c_master_transmit(device_handle_, buf, 2, -1);
    }

    void read(uint8_t* buf, const size_t len) {
        int timeout = -1;  // timeout of -1 means wait forever
        i2c_master_receive(device_handle_, buf, len, timeout);
    }
    void read_6(uint8_t* buf, uint8_t reg_addr) {
        i2c_master_transmit_receive(device_handle_, &reg_addr, 1, buf, 6, -1);
    }

   private:
    i2c_master_dev_handle_t device_handle_;
};