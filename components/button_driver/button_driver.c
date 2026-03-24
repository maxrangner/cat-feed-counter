#include "button_driver.h"

// #include <freertos/FreeRTOS.h>
// #include <freertos/task.h>
// #include <freertos/queue.h>
#include <driver/gpio.h>
#include <esp_attr.h>

static void IRAM_ATTR button_isr(void *arg)
{

}

void button_init(const button_cfg_t* cfg, button_t* button_handle)
{
    gpio_config_t io_config = {
        .intr_type = GPIO_INTR_ANYEDGE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << cfg->gpio_num),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_ENABLE,
    };
    gpio_config(&io_config);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(cfg->gpio_num, button_isr, button_handle);

    button_handle->timer = xTimerCreate(
        "btn_timer",
        0,
        pdFALSE,
        button_handle,
        NULL // Callback
    );
}