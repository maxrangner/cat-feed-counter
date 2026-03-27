#include "app_controller.h"

#include "lvgl.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "display.h"
#include "app_storage.h"


static const char *TAG = "app_controller";
static void btn_cb(button_event_t btn_event, void* user_data);

AppController::AppController() : stats_{} {}

void AppController::init()
{
    display_init();

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    lvgl_port_lock(portMAX_DELAY);
        main_screen_init();
        statistics_screen_init();
    lvgl_port_unlock();

    current_screen_ = &main_screen;
    counter = 0;

    in_queue_ = xQueueCreate(10, sizeof(app_event_t));

    xTaskCreate(
        app_task,
        "appTask",
        8192,
        this,
        1,
        &task_app_controller_
    );

    button_cfg_t btn_cfg = {
        .gpio_num = 9,
        .hasPullup = true,
        .debounce = 100,
        .btn_callback = btn_cb,
        .user_data = this,
    };
    button_init(&btn_cfg, &btn_);
}

void AppController::app_task(void* pvParameters)
{
    auto* self = static_cast<AppController*>(pvParameters);

    esp_reset_reason_t reason = esp_reset_reason();
    ESP_LOGI(TAG, "Reset reason: %d", reason);

    AppWifi app_wifi;
    AppStorage app_storage;
    
    app_wifi.init(self->getAppQueue());
    app_storage.init();
    app_storage.load_stats(&self->stats_);
    ESP_LOGI(TAG, "Loaded total feeds: %d", self->stats_.tot_num_feeds);
    
    app_event_t event;
    day_data_t temp_data = {
        .num_feeds = 1,
    };

    app_storage.write_stats(&temp_data);
    lvgl_port_lock(0);
        self->current_screen_->enter();
    lvgl_port_unlock();

    while(1) {
        // ESP_LOGI(TAG, "app controller says hello! counter = %d", self->counter);
        // if (self->counter % 5 == 0) {
        //     self->current_screen_ = &statistics_screen;
        // } else {
        //     self->current_screen_ = &main_screen;
        // }
        // btn_cb(btn_event, (void*)self);
        // vTaskDelay(pdMS_TO_TICKS(50));
        if (xQueueReceive(self->in_queue_, &event, portMAX_DELAY)) {
            if (event.msg_event == AppEventType::BTN_SHORT_PRESS) {
                lvgl_port_lock(0);
                    self->current_screen_->enter();
                    self->counter = (self->counter + 1) % 10;
                    self->current_screen_->render(self->counter);
                lvgl_port_unlock();
            }
            if (event.msg_event == AppEventType::BTN_LONG_PRESS) {
            }
            if (event.msg_event == AppEventType::BTN_LONG_PRESS) {
                // PLACEHOLDER
            }
            if (event.msg_event == AppEventType::WIFI_UPDATE) {
                if (event.wifi_state == WifiState::CONNECTED_STA) {
                    ESP_LOGI(TAG, "WifiState::CONNECTED_STA");
                }
                if (event.wifi_state == WifiState::DISCONNECTED) {
                    ESP_LOGI(TAG, "WifiState::DISCONNECTED");
                }
                
            }
        }
    }
}

void AppController::post_event(app_event_t event)
{
    xQueueSend(in_queue_, &event, 0);
}

void btn_cb(button_event_t btn_event, void* user_data)
{
    AppController *self = (AppController *)user_data;

    app_event_t event;

    if (btn_event == BTN_SHORT_PRESS) {
        event.msg_event = AppEventType::BTN_SHORT_PRESS;
    } else {
        event.msg_event = AppEventType::BTN_LONG_PRESS;
    }
    self->post_event(event);
}

QueueHandle_t AppController::getAppQueue()
{
    return in_queue_;
}
