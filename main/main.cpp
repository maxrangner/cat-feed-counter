#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "app_controller.h"


extern "C" void app_main(void)
{
    AppController app_controller;
    app_controller.init();

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(portMAX_DELAY));
    }
}