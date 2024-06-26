/*
---------------------------------------
- SIGUELINEAS CURSO LABORATORIO GLUON -
-          TIWTCH.TV/LABGLUON         -
---------------------------------------

pinout.h
TB6612_"nombre_pin"  --> TB6612_PWMA, TB6612_AIN1, TB6612_AIN2, TB6612_PWMB, TB6612_BIN1, TB6612_BIN2, TB6612_STBY
MULTPLX_"nombre_pin" --> MULTPLX_SIG, MULTPLX_S0, MULTPLX_S1, MULTPLX_S2, MULTPLX_S3, MULTPLX_EN
*/

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <esp_log.h>
#include <esp_timer.h>
#include <pinout.h>
#include <TB6612.h>
#include <CD74HC4067.h>

extern "C" void app_main();

TB6612 motores(TB6612_AIN1, TB6612_AIN2, TB6612_PWMA, TB6612_BIN1, TB6612_BIN2, TB6612_PWMB, TB6612_STBY, 0, 0);
CD74HC4067 sensores (MULTPLX_SIG, MULTPLX_S0, MULTPLX_S1, MULTPLX_S2, MULTPLX_S3, MULTPLX_EN);

static TaskHandle_t core_A = NULL;
static TaskHandle_t core_B = NULL;

static char TAG[] = "MAIN";

void coreAThread(void *arg)
{
    ESP_LOGE(TAG, "Iniciando CORE A");
 
    while (true)
    {
        motores.Update(TB6612::FWD,100,100);
    }
}


void coreBThread(void *arg)
{
    ESP_LOGE(TAG, "Iniciando CORE B");
    int sensor[8] = {};
    while (true)
    {
        for (uint8_t i=0; i<8; i++)
        {
            sensores.ReadSensor(i, sensor[i]);
        }
        printf("%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\n", sensor[0], sensor[1], sensor[2], sensor[3], sensor[4], sensor[5], sensor[6], sensor[7]);
    }
}

void app_main() 
{
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << KEY);
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    bool start = 0;
    while(!start)
    {
        start = gpio_get_level(KEY);
    }
    vTaskDelay(pdMS_TO_TICKS(1000));

    xTaskCreatePinnedToCore(coreBThread, "core_B", 4096, NULL, 9, &core_B, 1);    
    xTaskCreatePinnedToCore(coreAThread, "core_A", 4096, NULL, 10, &core_A, 0);
}