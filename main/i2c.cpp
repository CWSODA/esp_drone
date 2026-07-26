#include "i2c.hpp"

i2c_master_bus_handle_t bus_handle;

// only run once unless more i2c ports are needed
void init_i2c_bus() {
    auto bus_cfg = i2c_master_bus_config_t{};
    bus_cfg.i2c_port = 0;
    bus_cfg.sda_io_num = SDA_PIN;
    bus_cfg.scl_io_num = SCL_PIN;
    bus_cfg.clk_source = I2C_CLK_SRC_DEFAULT;
    bus_cfg.glitch_ignore_cnt = 7;
    bus_cfg.intr_priority = 0;
    bus_cfg.trans_queue_depth = 0;  // typically #device * byte per transfer
    bus_cfg.flags.enable_internal_pullup = true;
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_cfg, &bus_handle));
}

class I2CDevice {
   public:
    I2CDevice() {}

    void init(uint16_t addr) {
        auto device_cfg = i2c_device_config_t{};
        device_cfg.dev_addr_length = I2C_ADDR_BIT_LEN_7;
        device_cfg.device_address = addr;
        device_cfg.scl_speed_hz = 100'000;
        device_cfg.scl_wait_us = 0;  // timeout, use default val
        device_cfg.flags.disable_ack_check = false;

        ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &device_cfg,
                                                  &device_handle_));
    }

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

   public:
    i2c_master_dev_handle_t device_handle_;
};