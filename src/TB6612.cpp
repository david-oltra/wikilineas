#include <driver/gpio.h>
#include <TB6612.h>
#include <driver/ledc.h>

TB6612::TB6612(gpio_num_t _MOTOR_A_1, gpio_num_t _MOTOR_A_2, gpio_num_t _MOTOR_A_PWM, gpio_num_t _MOTOR_B_1, gpio_num_t _MOTOR_B_2, gpio_num_t _MOTOR_B_PWM, int _PWM_MOTOR_A = 50, int _PWM_MOTOR_B = 50):
    MOTOR_A_1 (_MOTOR_A_1),
    MOTOR_A_2 (_MOTOR_A_2),
    MOTOR_B_1 (_MOTOR_B_1),
    MOTOR_B_2 (_MOTOR_B_2),
    MOTOR_A_PWM (_MOTOR_A_PWM),
    MOTOR_B_PWM (_MOTOR_B_PWM),
    PWM_MOTOR_A (_PWM_MOTOR_A),
    PWM_MOTOR_B (_PWM_MOTOR_B)
{
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << MOTOR_A_1) | (1ULL << MOTOR_A_2) | (1ULL << MOTOR_B_1) | (1ULL << MOTOR_B_2) | (1ULL << MOTOR_A_PWM) | (1ULL << MOTOR_B_PWM);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    gpio_set_level(MOTOR_A_1, 0);
    gpio_set_level(MOTOR_A_2, 0);
    gpio_set_level(MOTOR_B_1, 0);
    gpio_set_level(MOTOR_B_2, 0);

    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .duty_resolution  = LEDC_TIMER_10_BIT ,
        .timer_num        = LEDC_TIMER_0,
        .freq_hz          = 10000,  // Set output frequency at 10 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .gpio_num       = MOTOR_A_PWM,
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = LEDC_TIMER_0,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    ledc_channel.gpio_num = MOTOR_B_PWM;
    ledc_channel.channel = LEDC_CHANNEL_1;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    // Set duty to 50% (0 to 1024 (10bits))
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, PWM_MOTOR_A*1024/100));
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, PWM_MOTOR_B*1024/100));
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1));
}

void TB6612::Update(enum DIRECTION dir, int PWM_MOTOR_A, int PWM_MOTOR_B)
{
    switch(dir)
    {
        case TB6612::STOP:
            gpio_set_level(MOTOR_A_1,1);
            gpio_set_level(MOTOR_A_2,1);
            gpio_set_level(MOTOR_B_1,1);
            gpio_set_level(MOTOR_B_2,1);
            break;
        case TB6612::FWD:
            gpio_set_level(MOTOR_A_1,0);
            gpio_set_level(MOTOR_A_2,1);
            gpio_set_level(MOTOR_B_1,0);
            gpio_set_level(MOTOR_B_2,1);
            break;
        case TB6612::LEFT:
            gpio_set_level(MOTOR_A_1,1);
            gpio_set_level(MOTOR_A_2,0);
            gpio_set_level(MOTOR_B_1,0);
            gpio_set_level(MOTOR_B_2,1);
            break;
        case TB6612::RIGHT:
            gpio_set_level(MOTOR_A_1,0);
            gpio_set_level(MOTOR_A_2,1);
            gpio_set_level(MOTOR_B_1,1);
            gpio_set_level(MOTOR_B_2,0);
            break;
        default:
            break;
    }
    // Set duty)
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, PWM_MOTOR_A*1024/100));
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, PWM_MOTOR_B*1024/100));
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1));
}