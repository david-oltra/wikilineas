#include <CD74HC4067.h>
#include <driver/gpio.h>
#include <driver/adc.h>

CD74HC4067::CD74HC4067(gpio_num_t _SIG, gpio_num_t _S0, gpio_num_t _S1,
                     gpio_num_t _S2, gpio_num_t _S3, gpio_num_t _EN):
        SIG (_SIG),
        S0 (_S0),
        S1 (_S1),
        S2 (_S2),
        S3 (_S3), 
        EN (_EN)
{
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << S0) | (1ULL << S1) | (1ULL << S2) | (1ULL << S3) | (1ULL << EN);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_11);
}

int CD74HC4067::ReadSensor( int sensor, int data)
{
    uint8_t val[8][4] = {{0,0,0,0},
                        {0,0,0,1},
                        {0,0,1,0},
                        {0,0,1,1},
                        {0,1,0,0},
                        {0,1,0,1},
                        {0,1,1,0},
                        {0,1,1,1}};
    
    gpio_set_level(S0,val[sensor][0]);
    gpio_set_level(S1,val[sensor][1]);
    gpio_set_level(S2,val[sensor][3]);
    gpio_set_level(S3,val[sensor][4]);

    data = adc1_get_raw(ADC1_CHANNEL_0);
    return data;
}