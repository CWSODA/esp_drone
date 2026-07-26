#pragma once

#include "driver/i2c_master.h"

constexpr i2c_port_num_t I2C_PORT = 0;
constexpr gpio_num_t SDA_PIN = GPIO_NUM_21;
constexpr gpio_num_t SCL_PIN = GPIO_NUM_22;

i2c_master_bus_handle_t bus_handle;

// only run once unless more i2c ports are needed
void init_i2c_bus();

class I2CDevice {
   public:
    I2CDevice(uint16_t addr) {
        auto device_cfg = i2c_device_config_t{};
        device_cfg.dev_addr_length = I2C_ADDR_BIT_LEN_7;
        device_cfg.device_address = addr;
        device_cfg.scl_speed_hz = 100'000;
        device_cfg.scl_wait_us = 0;  // timeout, use default val

        ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &device_cfg,
                                                  &device_handle_));
    }

    void write(const uint8_t* buf, const size_t len) {
        int timeout = -1;  // timeout of -1 means wait forever
        i2c_master_transmit(device_handle_, buf, len, timeout);
    }

    void read(uint8_t* buf, const size_t len) {
        int timeout = -1;  // timeout of -1 means wait forever
        i2c_master_receive(device_handle_, buf, len, timeout);
    }

   private:
    i2c_master_dev_handle_t device_handle_;
};