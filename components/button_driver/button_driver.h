#ifndef BUTTON_DRIVER_H
#define BUTTON_DRIVER_H

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    BTN_SHORT_PRESS,
    BTN_LONG_PRESS,
} button_event_t;

typedef struct {
    uint8_t gpio_num;
    bool pressed_level;
    void (*btn_callback)(button_event_t event, void* user_data);
    void* user_data;
    TimerHandle_t timer;
    uint64_t press_time;
} button_t;

typedef struct {
    uint8_t gpio_num;
    bool hasPullup;
    uint16_t debounce;
    void (*btn_callback)(button_event_t event, void* user_data);
    void* user_data;
} button_cfg_t;

void button_init(const button_cfg_t* cfg, button_t* button_handle);

#ifdef __cplusplus
}

#endif
#endif