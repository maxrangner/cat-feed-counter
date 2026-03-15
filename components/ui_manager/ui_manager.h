#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class UiManager {
    TaskHandle_t ui_task_ = nullptr;
    QueueHandle_t ui_in_queue_ = nullptr;
    static void ui_task(void* pvParameters);
public:
    UiManager(void);
    void init(void);
};

#endif