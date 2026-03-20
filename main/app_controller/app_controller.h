#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "screens.h"

enum class AppEventType {
    ButtonShortPress,
    ButtonLongPress,
};

typedef struct {
    AppEventType msg_event;
} app_event;

class AppController {
    TaskHandle_t task_app_controller_ = nullptr;
    QueueHandle_t in_queue_ = nullptr;
    screen_t* current_screen_;

    uint8_t counter;

    static void app_task(void* pvParameters);
public:
    AppController(void);
    void init(void);
};

#endif