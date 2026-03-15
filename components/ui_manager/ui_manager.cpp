#include "ui_manager.h"

#include "esp_lvgl_port.h"
#include "lvgl.h"
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
    bool flipped = false;

    while(1) {
        ESP_LOGI(TAG, "ui_manager says hello");
        lvgl_port_lock(portMAX_DELAY);

            lv_refr_now(lv_disp_get_default());
            if (flipped) {
                lv_disp_set_rotation(lv_disp_get_default(), LV_DISP_ROT_180);
                flipped = !flipped;
            } else {
                lv_disp_set_rotation(lv_disp_get_default(), LV_DISP_ROT_NONE);
                flipped = !flipped;
            }

        lvgl_port_unlock();
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
