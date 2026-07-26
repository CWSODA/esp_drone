#include "driver/mcpwm_timer.h"
#include "driver/mcpwm_oper.h"
#include "driver/mcpwm_cmpr.h"
#include "driver/mcpwm_gen.h"

using uint = unsigned int;

constexpr float RES = 1.0f * 1e-6;  // 1 us resolution
constexpr uint32_t RES_HZ = 1.0f / RES;

constexpr uint FREQ = 50;
constexpr uint32_t PERIOD_TICKS = (1.0f / FREQ) / RES;  // period / res

constexpr uint16_t MAX_COMP_VAL = 2400;
constexpr uint16_t MIN_COMP_VAL = 1100;

class PWMTimer {
   public:
    // group id should be either 0 or 1 with 3 timers per group
    PWMTimer() {}

    void init(int group_id) {
        auto timer_cfg = mcpwm_timer_config_t{};
        timer_cfg.group_id = group_id;
        timer_cfg.clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT;
        timer_cfg.resolution_hz = RES_HZ;
        timer_cfg.count_mode = MCPWM_TIMER_COUNT_MODE_UP;
        timer_cfg.period_ticks = PERIOD_TICKS;
        timer_cfg.intr_priority = 0;

        ESP_ERROR_CHECK(mcpwm_new_timer(&timer_cfg, &handle_));
    }

    mcpwm_timer_handle_t get_handle() { return handle_; }

    ~PWMTimer() { mcpwm_del_timer(handle_); }

   private:
    mcpwm_timer_handle_t handle_;
};

class PWMBlock {
   public:
    void init(const mcpwm_timer_handle_t timer_handle, const int pin) {
        auto oper_cfg = mcpwm_operator_config_t{};
        oper_cfg.group_id = 0;
        oper_cfg.intr_priority = 0;

        auto comp_cfg = mcpwm_comparator_config_t{};
        comp_cfg.intr_priority = 0;
        comp_cfg.flags.update_cmp_on_tez = true;

        auto gen_cfg = mcpwm_generator_config_t{};
        gen_cfg.gen_gpio_num = pin;

        ESP_ERROR_CHECK(mcpwm_new_operator(&oper_cfg, &oper_handle));
        ESP_ERROR_CHECK(
            mcpwm_new_comparator(oper_handle, &comp_cfg, &comp_handle));
        ESP_ERROR_CHECK(
            mcpwm_new_generator(oper_handle, &gen_cfg, &gen_handle));

        // initial compare value
        uint32_t ticks = 10;
        ESP_ERROR_CHECK(
            mcpwm_operator_connect_timer(oper_handle, timer_handle));
        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comp_handle, ticks));

        // on timer count zero when counting up, set output to high
        auto gen_timer_action = mcpwm_gen_timer_event_action_t{};
        gen_timer_action.direction = MCPWM_TIMER_DIRECTION_UP;
        gen_timer_action.event = MCPWM_TIMER_EVENT_EMPTY;
        gen_timer_action.action = MCPWM_GEN_ACTION_HIGH;
        ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(
            gen_handle, gen_timer_action));

        // on compare match, set output to low
        auto gen_comp_action = mcpwm_gen_compare_event_action_t{};
        gen_comp_action.direction = MCPWM_TIMER_DIRECTION_UP;
        gen_comp_action.comparator = comp_handle;
        gen_comp_action.action = MCPWM_GEN_ACTION_LOW;
        ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(
            gen_handle, gen_comp_action));

        ESP_ERROR_CHECK(mcpwm_timer_enable(timer_handle));
        ESP_ERROR_CHECK(
            mcpwm_timer_start_stop(timer_handle, MCPWM_TIMER_START_NO_STOP));
    }

    void set_compare_val(uint32_t count) {
        mcpwm_comparator_set_compare_value(comp_handle, count);
    }

    // sets power lvl in range [0,100]
    void set_power(float duty) {
        duty = duty / 100.0f;  // map to [0,1]
        uint32_t count = (MAX_COMP_VAL - MIN_COMP_VAL) * duty + MIN_COMP_VAL;
        printf("Compare count: %lu\n", count);
        set_compare_val(count);
    }

    void set_us(float us) { set_compare_val(us); }

    ~PWMBlock() {
        // free all handles
        mcpwm_del_operator(oper_handle);
        mcpwm_del_comparator(comp_handle);
        mcpwm_del_generator(gen_handle);
    }

   private:
    mcpwm_oper_handle_t oper_handle;
    mcpwm_cmpr_handle_t comp_handle;
    mcpwm_gen_handle_t gen_handle;
};