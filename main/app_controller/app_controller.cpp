#include "app_controller.h"

#include <ctime>
#include "lvgl.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_pm.h"

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
        .debounce = 50,
        .long_press_dur = 500,
        .btn_callback = btn_cb,
        .user_data = this,
    };
    button_init(&btn_cfg, &btn_);

    /*
    Power management to keep device heat low
    TODO: verify min_freq_mhz — find lowest value that keeps display smooth
    */
    esp_pm_config_t pm_config = {
        .max_freq_mhz = 160,
        .min_freq_mhz = 80,
        .light_sleep_enable = true,
    };
    esp_pm_configure(&pm_config);
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

    // button_event_t btn_event = BTN_SHORT_PRESS;

    while(1) {
        // ESP_LOGI(TAG, "app controller says hello! counter = %d", self->counter);
        // if (self->counter % 5 == 0) {
        //     self->current_screen_ = &statistics_screen;
        // } else {
        //     self->current_screen_ = &main_screen;
        // }
        // btn_cb(btn_event, (void*)self);
        // vTaskDelay(pdMS_TO_TICKS(1000));
        if (xQueueReceive(self->in_queue_, &event, portMAX_DELAY)) {
            if (event.msg_event == AppEventType::BTN_SHORT_PRESS) {
                ESP_LOGI(TAG, "BTN_SHORT_PRESS");
                lvgl_port_lock(0);
                    self->current_screen_->enter();
                    self->counter = (self->counter + 1) % 10;
                    self->current_screen_->render(self->counter);
                lvgl_port_unlock();
            }
            if (event.msg_event == AppEventType::BTN_LONG_PRESS) {
                ESP_LOGI(TAG, "BTN_LONG_PRESS");
                app_wifi.connect();
            }
            if (event.msg_event == AppEventType::TIME_SYNC_STATUS) {
                int64_t t = time(NULL);
                ESP_LOGI(TAG, "TIME_SYNC_STATUS: %lld", t);

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
    }
    if (btn_event == BTN_LONG_PRESS) {
        event.msg_event = AppEventType::BTN_LONG_PRESS;
    }
    self->post_event(event);
}

QueueHandle_t AppController::getAppQueue()
{
    return in_queue_;
}
