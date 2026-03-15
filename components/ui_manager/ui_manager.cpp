#include "ui_manager.h"

#include "esp_log.h"

static const char *TAG = "ui manager";

UiManager::UiManager(void) {}

void UiManager::init()
{
    xTaskCreatePinnedToCore(       // UI Task
        ui_task,                   // Function to implement the task
        "uiTask",                  // Name of the task
        8192,                      // Stack size in words
        this,                      // Task input parameter
        1,                         // Priority of the task
        &ui_task_,                 // Task handle.
        0                          // Core where the task should run
    );
}

void UiManager::ui_task(void* pvParameters)
{
    auto* self = static_cast<UiManager*>(pvParameters);

    while(1) {
        ESP_LOGI(TAG, "ui_manager says hello");
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}
