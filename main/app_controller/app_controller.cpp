#include "app_controller.h"

#include "esp_log.h"

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
}

void AppController::app_task(void* pvParameters)
{
    auto* self = static_cast<AppController*>(pvParameters);

    while(1) {
        ESP_LOGI(TAG, "app controller says hello!");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}