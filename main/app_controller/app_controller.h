#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include <cstdint>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "app_types.h"
#include "screens.h"
#include "button_driver.h"

class AppController {
    TaskHandle_t task_app_controller_ = nullptr;
    QueueHandle_t in_queue_ = nullptr;
    
    Screen* current_screen_;
    MainScreen main_screen_;
    OptionsScreen options_screen_;
    Screen* screens[2] = {&main_screen_, &options_screen_};

    stats_t stats_;
    button_t btn_;
    uint8_t counter;

    static void app_task(void* pvParameters);
public:
    AppController();
    void init();
    void post_event(app_event_t event);
    QueueHandle_t getAppQueue();
    void next_screen();
};

#endif