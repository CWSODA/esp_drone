#include "pwm.cpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*
Each motor uses approximately 2.2A at 5V
*/

constexpr uint32_t ARM_PULSE_WIDTH = 900;  // in µs

constexpr int FL_PIN = 4;
constexpr int FR_PIN = 4;
constexpr int BL_PIN = 4;
constexpr int BR_PIN = 4;

class MotorCtrl {
   public:
    MotorCtrl() {
        timers_[0].init(0);
        timers_[1].init(1);

        FL_.init(timers_[0].get_handle(), FL_PIN);
        // FR_.init(timers_[0].get_handle(), FR_PIN);

        // BL_.init(timers_[1].get_handle(), BL_PIN);
        // BR_.init(timers_[1].get_handle(), BR_PIN);
    }

    void arm_motors() {
        // must be set to this and wait for a few seconds
        for (auto& motor : motor_array_) {
            motor.set_us(ARM_PULSE_WIDTH);
        }
        vTaskDelay(pdMS_TO_TICKS(10000));
    }

    void gradient_power() {
        static float power = 0.0f;
        static float incr = 1.0f;

        power += incr;
        if (power >= 100.0f)
            incr = -1.0f;
        else if (power <= 0.0f)
            incr = 1.0f;

        for (auto& motor : motor_array_) {
            motor.set_power(power);
        }
    }

    //    private:
    PWMTimer timers_[2];

    PWMBlock motor_array_[1] = {};
    PWMBlock& FL_ = motor_array_[0];
    // PWMBlock& FR_ = motor_array_[1];
    // PWMBlock& BL_ = motor_array_[2];
    // PWMBlock& BR_ = motor_array_[3];
};