#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

enum class sys_msg_type_t {
    BUTTON_UPDATE
};

typedef struct {
    sys_msg_type_t msg_type;
} sys_msg_t;

class SystemManager {
    TaskHandle_t system_task_ = nullptr;
    QueueHandle_t system_in_queue_ = nullptr;
    QueueHandle_t ui_in_queue_ = nullptr;

    static void system_task(void* pvParameters);
public:
    SystemManager(void);
    void init(void);
    void connectUi(QueueHandle_t ui_queue);
    QueueHandle_t getInputQueue(void);
};

#endif