#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#define NUM_SCREENS 2

#include <cstdint>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "app_types.h"
#include "screens.h"
#include "button_driver.h"

class AppController {
    TaskHandle_t task_app_controller_ = nullptr;
    QueueHandle_t in_queue_ = nullptr;

    settings_t settings_;
    stats_t stats_;
    button_t btn_;
    uint8_t counter;

    Screen* current_screen_;
    MainScreen main_screen_;
    OptionsScreen options_screen_;
    Screen* screens[NUM_SCREENS] = {&main_screen_, &options_screen_};

    static void app_task(void* pvParameters);
    void handle_screen_action(ScreenAction action);
    void next_screen();
public:
    AppController();
    void init();
    void post_event(app_event_t event);
    QueueHandle_t getAppQueue();
};

#endif