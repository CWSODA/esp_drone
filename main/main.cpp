#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "i2c.hpp"
#include "motor.cpp"
#include "imu.hpp"
#include "input.hpp"

#include <string>

extern "C" {
void app_main(void) {
    MotorCtrl motor_ctrl;
    printf("Arming motors\n");
    // motor_ctrl.arm_motors();

    printf("Starting gradient\n");
    char buf[1024];
    std::string str;
    while (true) {
        if (fgets(buf, sizeof(buf), stdin) != nullptr) {
            str.append(buf);

            int idx = str.find('\n');
            if (idx == std::string::npos) continue;
            std::string line = str.substr(0, idx);

            float val;
            if (sscanf(str.data(), "%f\n", &val) != 1) continue;
            printf("Setting to %f us\n", val);
            motor_ctrl.FL_.set_us(val);

            str = str.substr(idx + 1);
        }
        // motor_ctrl.gradient_power();
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    // init I2C
    init_i2c_bus();

    // set up sensors
    TaskHandle_t sensor_task;
    xTaskCreatePinnedToCore(sensor_manager, "sensors", 4096, NULL, 1, &sensor_task, tskNO_AFFINITY);

    // MotorCtrl motor_ctrl;
    // motor_ctrl.arm_motors();

    while (true) {
        process_input();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
}
// esp_restart();