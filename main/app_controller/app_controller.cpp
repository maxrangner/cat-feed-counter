#include "app_controller.h"

#include "lvgl.h"
#include "esp_log.h"
#include "display.h"
#include "screens.h"

static const char *TAG = "app_controller";

AppController::AppController(void) {}

void AppController::init(void)
{
    xTaskCreatePinnedToCore(       // UI Task
        app_task,                  // Function to implement the task
        "appTask",                 // Name of the task
        8192,                      // Stack size in words
        this,                      // Task input parameter
        1,                         // Priority of the task
        &task_app_controller_,     // Task handle.
        0                          // Core where the task should run
    );

    display_init();

    lvgl_port_lock(0);
        main_screen_init();
    lvgl_port_unlock();
}

void AppController::app_task(void* pvParameters)
{
    auto* self = static_cast<AppController*>(pvParameters);
    AppEventType event;

    while(1) {
        ESP_LOGI(TAG, "app controller says hello!");
        if (xQueueReceive(self->in_queue_, &event, portMAX_DELAY)) {
            if (event == AppEventType::ButtonShortPress) {

            }
            if (event == AppEventType::ButtonLongPress) {

            }
        }
    }
}
