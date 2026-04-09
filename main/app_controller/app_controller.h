#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#define NUM_SCREENS 2

#include <cstdint>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "app_types.h"
#include "screens.h"
#include "button_driver.h"
#include "app_storage.h"

typedef struct {
    settings_t settings;
    stats_t stats;
    Screen* current_screen;
    day_data_t today;
} app_state_t;

class AppController {
    TaskHandle_t task_app_controller_ = nullptr;
    QueueHandle_t in_queue_ = nullptr;

    AppWifi app_wifi_;
    AppStorage app_storage_;
    app_state_t app_state_;
    button_t btn_;
    MainScreen main_screen_;
    OptionsScreen options_screen_;
    Screen* screens[NUM_SCREENS] = {&main_screen_, &options_screen_};

    static void app_task(void* pvParameters);
    void handle_app_events(app_event_t event);
    void next_screen();
    void increment_count();
    void save_data();
    void option_1_action();
    void option_2_action();
    void option_3_action();
public:
    AppController();
    void init();
    void post_event(app_event_t event);
    QueueHandle_t getAppQueue();
    void reset_day();
};

#endif