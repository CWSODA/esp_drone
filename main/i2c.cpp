#include "i2c.hpp"

i2c_master_bus_handle_t bus_handle;
void init_i2c_bus() {
    auto bus_cfg = i2c_master_bus_config_t{};
    bus_cfg.i2c_port = I2C_PORT;
    bus_cfg.sda_io_num = SDA_PIN;
    bus_cfg.scl_io_num = SCL_PIN;
    bus_cfg.clk_source = I2C_CLK_SRC_DEFAULT;
    bus_cfg.glitch_ignore_cnt = 7;
    bus_cfg.intr_priority = 0;
    bus_cfg.trans_queue_depth = 0;  // typically #device * byte per transfer
    bus_cfg.flags.enable_internal_pullup = true;
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_cfg, &bus_handle));
}

I2CDevice::I2CDevice(uint16_t addr) {
    auto device_cfg = i2c_device_config_t{};
    device_cfg.dev_addr_length = I2C_ADDR_BIT_LEN_7;
    device_cfg.device_address = addr;
    device_cfg.scl_speed_hz = 100'000;
    device_cfg.scl_wait_us = 0;  // timeout, use default val

    ESP_ERROR_CHECK(
        i2c_master_bus_add_device(bus_handle, &device_cfg, &device_handle_));
}

void I2CDevice::init(uint16_t addr) {
    auto device_cfg = i2c_device_config_t{};
    device_cfg.dev_addr_length = I2C_ADDR_BIT_LEN_7;
    device_cfg.device_address = addr;
    device_cfg.scl_speed_hz = 100'000;
    device_cfg.scl_wait_us = 0;  // timeout, use default val
    device_cfg.flags.disable_ack_check = false;

    ESP_ERROR_CHECK(
        i2c_master_bus_add_device(bus_handle, &device_cfg, &device_handle_));
}