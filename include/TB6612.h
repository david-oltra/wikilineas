#ifndef __TB6612_H__
#define __TB6612_H__


class TB6612
{
    public:

        TB6612(gpio_num_t _MOTOR_A_1, gpio_num_t _MOTOR_A_2, gpio_num_t _MOTOR_A_PWM, 
               gpio_num_t _MOTOR_B_1, gpio_num_t _MOTOR_B_2, gpio_num_t _MOTOR_B_PWM,
               int _PWM_MOTOR_A, int _PWM_MOTOR_B);

        enum DIRECTION
        {
            STOP = 0,
            FWD,
            LEFT,
            RIGHT,
            BACK
        };

        void Update(enum DIRECTION dir, int PWM_MOTOR_A, int PWM_MOTOR_B);
        /*
        1 1 1 1 -> STOP
        0 1 0 1 -> FWD
        1 0 1 0 -> BACK
        1 0 0 1 -> IZQ
        0 1 1 0 -> DRCH
        */

    private:
        gpio_num_t MOTOR_A_1, MOTOR_A_2, MOTOR_B_1, MOTOR_B_2, MOTOR_A_PWM, MOTOR_B_PWM;
        int PWM_MOTOR_A, PWM_MOTOR_B;

};

#endif