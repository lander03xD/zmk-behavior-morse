#define DT_DRV_COMPAT zmk_behavior_ir_tv

#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>

LOG_MODULE_REGISTER(behavior_ir_tv, CONFIG_ZMK_LOG_LEVEL);

// --- Panasonic/Kaseikyo timing (microseconds) ---
#define PANA_HEADER_MARK   3500
#define PANA_HEADER_SPACE  1700
#define PANA_BIT_MARK       450
#define PANA_ZERO_SPACE     420
#define PANA_ONE_SPACE     1280
#define PANA_TRAIL_MARK      450

// --- TV address + power command ---
#define PANA_ADDRESS   0x400401u  // 24 bits
#define PANA_POWER_CMD 0x00BCBDu  // 24 bits

#define IR_CARRIER_FREQ_HZ 38000
#define IR_DUTY_CYCLE_PCT  33
#define IR_FRAME_MAX 98

static const struct pwm_dt_spec ir_pwm = PWM_DT_SPEC_GET(DT_NODELABEL(pwm_ir));

static int build_panasonic_frame(uint32_t address, uint32_t command,
                                   uint32_t *out, size_t out_max) {
    // in our case this should always return the same thing because we only have one button that can be pressed
    size_t idx = 0;

    out[idx++] = PANA_HEADER_MARK;
    out[idx++] = PANA_HEADER_SPACE;

    for (int field = 0; field < 2; field++) {
        uint32_t value = (field == 0) ? address : command;
        for (int bit = 0; bit < 24; bit++) {
            if (idx + 2 > out_max) {
                return -1;
            }
            bool is_one = (value >> bit) & 0x1;
            out[idx++] = PANA_BIT_MARK;
            out[idx++] = is_one ? PANA_ONE_SPACE : PANA_ZERO_SPACE;
        }
    }

    out[idx++] = PANA_TRAIL_MARK;
    return (int)idx;
}

static int send_ir_sequence(const uint32_t *seq, size_t len) {
    if (!pwm_is_ready_dt(&ir_pwm)) {
        LOG_ERR("PWM device not ready");
        return -ENODEV;
    }

    uint32_t period_ns = 1000000000 / IR_CARRIER_FREQ_HZ;
    uint32_t pulse_ns = (period_ns * IR_DUTY_CYCLE_PCT) / 100;

    for (size_t i = 0; i < len; i++) {
        bool is_mark = (i % 2 == 0); 
        if (is_mark) {
            pwm_set_dt(&ir_pwm, PWM_NSEC(period_ns), PWM_NSEC(pulse_ns));
        } else {
            pwm_set_dt(&ir_pwm, PWM_NSEC(period_ns), PWM_NSEC(0));
        }
        k_busy_wait(seq[i]);
    }
    pwm_set_dt(&ir_pwm, PWM_NSEC(period_ns), PWM_NSEC(0));
    return 0;
}

static int behavior_ir_tv_binding_pressed(struct zmk_behavior_binding *binding,
                                                  struct zmk_behavior_binding_event event) {
    LOG_DBG("IR key pressed");
    uint32_t frame[IR_FRAME_MAX];
    int len = build_panasonic_frame(PANA_ADDRESS, PANA_POWER_CMD, frame, IR_FRAME_MAX);
    if (len < 0) {
        LOG_ERR("IR frame buffer too small");
        return -1;
    }
    return send_ir_sequence(frame, (size_t)len);
    LOG_DBG("IR sequence sent");
}

static int behavior_ir_tv_binding_released(struct zmk_behavior_binding *binding,
                                                   struct zmk_behavior_binding_event event) {
    return 0;
}

static const struct behavior_driver_api behavior_ir_tv_driver_api = {
    .binding_pressed = behavior_ir_tv_binding_pressed,
    .binding_released = behavior_ir_tv_binding_released,
};

#define IR_TV_INST(n)                                                                       \
    BEHAVIOR_DT_INST_DEFINE(n, NULL, NULL, NULL, NULL, POST_KERNEL,                                \
                             CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                                  \
                             &behavior_ir_tv_driver_api);

DT_INST_FOREACH_STATUS_OKAY(IR_TV_INST)
