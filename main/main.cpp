#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "i2c.hpp"
#include "motor.cpp"
#include "imu.cpp"

extern "C" {
void app_main(void) {
    // MotorCtrl motor_ctrl;
    // printf("Arming motors\n");
    // motor_ctrl.arm_motors();

    // printf("Starting gradient\n");
    // while (true) {
    //     motor_ctrl.gradient_power();
    //     vTaskDelay(pdMS_TO_TICKS(100));
    // }

    // init I2C
    init_i2c_bus();

    // set up sensors
    TaskHandle_t sensor_task;
    xTaskCreatePinnedToCore(sensor_manager, "sensors", 4096, NULL, 1,
                            &sensor_task, tskNO_AFFINITY);

    // MotorCtrl motor_ctrl;
    // motor_ctrl.arm_motors();

    while (true) {
        // printf("HI\n");
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}
}
// esp_restart();