#include "app_controller.h"

#include "lvgl.h"
#include "esp_log.h"
#include "display.h"

static const char *TAG = "app_controller";

AppController::AppController(void) {}

void AppController::init(void)
{
    display_init();

    lvgl_port_lock(portMAX_DELAY);
        main_screen_init();
        statistics_screen_init();
    lvgl_port_unlock();

    current_screen_ = &main_screen;
    counter = 0;

    in_queue_ = xQueueCreate(10, sizeof(app_event));

    xTaskCreatePinnedToCore(       // UI Task
        app_task,                  // Function to implement the task
        "appTask",                 // Name of the task
        8192,                      // Stack size in words
        this,                      // Task input parameter
        1,                         // Priority of the task
        &task_app_controller_,     // Task handle.
        0                          // Core where the task should run
    );
}

void AppController::app_task(void* pvParameters)
{
    auto* self = static_cast<AppController*>(pvParameters);
    AppEventType event;

    main_screen_enter();

    while(1) {
        ESP_LOGI(TAG, "app controller says hello!");
        if (self->counter % 5 == 0) {
            self->current_screen_ = &statistics_screen;
        } else {
            self->current_screen_ = &main_screen;
        }
        self->current_screen_->enter();
        self->current_screen_->render(self->counter++);
        vTaskDelay(pdMS_TO_TICKS(1000));
        // if (xQueueReceive(self->in_queue_, &event, portMAX_DELAY)) {
        //     if (event == AppEventType::ButtonShortPress) {

        //     }
        //     if (event == AppEventType::ButtonLongPress) {

        //     }
        // }
    }
}
