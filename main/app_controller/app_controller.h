#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "app_types.h"
#include "screens.h"

class AppController {
    TaskHandle_t task_app_controller_ = nullptr;
    QueueHandle_t in_queue_ = nullptr;
    screen_t* current_screen_;

    stats_t stats_;
    uint8_t counter;

    static void app_task(void* pvParameters);
public:
    AppController();
    void init();
    void post_event(app_event_t event);
    QueueHandle_t getAppQueue();
};

#endif