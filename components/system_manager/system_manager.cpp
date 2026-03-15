#include "system_manager.h"

#include "esp_log.h"

static const char *TAG = "system manager";

SystemManager::SystemManager(void) {}

void SystemManager::init(void)
{
    xTaskCreatePinnedToCore(       // UI Task
        system_task,               // Function to implement the task
        "systemTask",              // Name of the task
        8192,                      // Stack size in words
        this,                      // Task input parameter
        1,                         // Priority of the task
        &task_system_manager_,     // Task handle.
        0                          // Core where the task should run
    );
}

void SystemManager::system_task(void* pvParameters)
{
    auto* self = static_cast<SystemManager*>(pvParameters);

    while(1) {
        ESP_LOGI(TAG, "system_manager says hello!");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}