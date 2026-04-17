#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#define NUM_SCREENS 2

#include <cstdint>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

#include "app_types.h"
#include "screens.h"
#include "button_driver.h"
#include "app_storage.h"

class AppController {
    TaskHandle_t task_app_controller_ = nullptr;
    QueueHandle_t in_queue_ = nullptr;
    esp_timer_handle_t reset_day_timer;

    AppWifi app_wifi_;
    AppStorage app_storage_;
    app_state_t app_state_;
    button_t btn_;

    MainScreen main_screen_;
    OptionsScreen options_screen_;
    Screen* screens[NUM_SCREENS] = {&main_screen_, &options_screen_};

    static void app_task(void* pvParameters);
    static void reset_day_timer_cb(void* arg);
    void handle_app_events(app_event_t event);
    void next_screen();
    void set_reset_timer();
    void increment_count();
    void save_data();
    void change_brightness();
    void increment_feed_interval();
    void rotate_display();
public:
    AppController();
    void init();
    void post_event(app_event_t event);
    QueueHandle_t getAppQueue();
    void reset_day();
};

#endif