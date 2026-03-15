#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class SystemManager {
    TaskHandle_t task_system_manager_ = nullptr;
    QueueHandle_t system_in_queue_ = nullptr;

    static void system_task(void* pvParameters);
public:
    SystemManager(void);
    void init(void);
};

#endif