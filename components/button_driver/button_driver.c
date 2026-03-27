#include "button_driver.h"

// #include <freertos/FreeRTOS.h>
// #include <freertos/task.h>
// #include <freertos/queue.h>
#include <driver/gpio.h>
#include <esp_attr.h>
#include <esp_log.h>

static const char *TAG = "button_driver";

static void IRAM_ATTR button_isr(void *arg)
{
    button_t* handle = (button_t*)arg;

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTimerResetFromISR(handle->timer, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void button_timer_cb(TimerHandle_t xTimer)
{
    ESP_LOGI(TAG, "Button timer callback!");
    button_t* handle = (button_t*)pvTimerGetTimerID(xTimer);
    button_event_t event;

    if (gpio_get_level(handle->gpio_num) == handle->pressed_level) {
        event = BTN_SHORT_PRESS;
        handle->btn_callback(event, handle->user_data);
    }
}

void button_init(const button_cfg_t* cfg, button_t* button_handle)
{
    gpio_config_t io_config = {
        .intr_type = GPIO_INTR_ANYEDGE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << cfg->gpio_num),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = cfg->hasPullup ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE,
    };
    gpio_config(&io_config);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(cfg->gpio_num, button_isr, button_handle);

    button_handle->gpio_num = cfg->gpio_num;
    button_handle->pressed_level = cfg->hasPullup ? 0 : 1;
    button_handle->btn_callback = cfg->btn_callback;
    button_handle->user_data = cfg->user_data;
    button_handle->timer = xTimerCreate(
        "btn_timer",
        pdMS_TO_TICKS(cfg->debounce),
        pdFALSE,
        button_handle,
        button_timer_cb
    );
    button_handle->press_time = 0;
}