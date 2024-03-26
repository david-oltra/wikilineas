#ifndef __CD74HC4067_H__
#define __CD74HC4067_H__

#include "driver/gpio.h"

class CD74HC4067
{
    public:

        CD74HC4067 (gpio_num_t _SIG, gpio_num_t _S0, gpio_num_t _S1,
                     gpio_num_t _S2, gpio_num_t _S3, gpio_num_t _EN);
        
        int ReadSensor(int sensor, int data);


    private:

        gpio_num_t SIG, S0, S1, S2, S3, EN;
        int sensor, data;

};

#endif