#include "app_controller.h"

#include "lvgl.h"
#include "esp_log.h"
#include "display.h"

static const char *TAG = "app_controller";
static void btn_cb(void *user_data);

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

    while(1) {
        ESP_LOGI(TAG, "app controller says hello! counter = %d", self->counter);
        // if (self->counter % 5 == 0) {
        //     self->current_screen_ = &statistics_screen;
        // } else {
        //     self->current_screen_ = &main_screen;
        // }
        btn_cb((void*)self);
        vTaskDelay(pdMS_TO_TICKS(50));
        if (xQueueReceive(self->in_queue_, &event, portMAX_DELAY)) {
            if (event == AppEventType::ButtonShortPress) {
                self->current_screen_->enter();
                self->counter = (self->counter + 1) % 100;
                self->current_screen_->render(self->counter++);
            }
            if (event == AppEventType::ButtonLongPress) {
                // PLACEHOLDER
            }
        }
    }
}

void AppController::post_event(app_event event)
{
    xQueueSend(in_queue_, &event, 0);
}

void btn_cb(void *user_data)
{
    AppController *self = (AppController *)user_data;

    app_event event;
    event.msg_event = AppEventType::ButtonShortPress;
    self->post_event(event);
}
