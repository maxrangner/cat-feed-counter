#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

enum class ui_msg_type_t {
    FEED_UPDATE
};

typedef struct {
    ui_msg_type_t msg_type;
} ui_msg_t;

class UiManager {
    TaskHandle_t ui_task_ = nullptr;
    QueueHandle_t ui_in_queue_ = nullptr;
    QueueHandle_t system_in_queue_ = nullptr;

    static void ui_task(void* pvParameters);
public:
    UiManager(void);
    void init(void);
    void connectSystem(QueueHandle_t sys_queue);
    QueueHandle_t getInputQueue(void);
};

#endif