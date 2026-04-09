#ifndef APP_WIFI_H
#define APP_WIFI_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "esp_event.h"
#include "esp_wifi.h"

enum class WifiState {
    DISCONNECTED,
    CONNECTED_STA,
    // STARTING_AP,
    // AP_ACTIVE
};

class AppWifi {
    QueueHandle_t app_queue_;
    static void wifi_event_cb(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
    static void snpt_task(void* pvParameters);
public:
    AppWifi();
    void init(QueueHandle_t app_queue);
    void sync_time();
};


#endif
