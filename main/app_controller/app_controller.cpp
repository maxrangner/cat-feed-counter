#include "app_controller.h"

#include <ctime>
#include <inttypes.h>
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

    settings_ = {
        .landscape_orientation = true,
        .half_feed_steps = false,
        .feed_interval = 3,
        .animal = Animals::CAT,
    };

    lvgl_port_lock(portMAX_DELAY);
        main_screen_.init();
        options_screen_.init();
        options_screen_.update_settings(settings_);
    lvgl_port_unlock();

    current_screen_ = screens[1];
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

    button_service_init();
    button_cfg_t btn_cfg = {
        .gpio_num = 9,
        .hasPullup = true,
        .debounce = 25,
        .long_press_dur = 500,
        .btn_callback = btn_cb,
        .user_data = this,
    };
    button_init(&btn_cfg, &btn_);
}

void AppController::app_task(void* pvParameters)
{
    auto* self = static_cast<AppController*>(pvParameters);

    AppWifi app_wifi;
    AppStorage app_storage;
    
    app_wifi.init(self->getAppQueue());
    app_storage.init();
    app_storage.load_stats(&self->stats_);
    ESP_LOGI(TAG, "Loaded total feeds: %" PRIu32, self->stats_.tot_num_feeds);
    
    app_event_t event;
    day_data_t temp_data = {
        .num_feeds = 1,
    };

    app_storage.write_stats(&temp_data);

    lvgl_port_lock(portMAX_DELAY);
        self->current_screen_->show();
    lvgl_port_unlock();

    display_set_brightness(30); // %

    while(1) {
        if (xQueueReceive(self->in_queue_, &event, portMAX_DELAY)) {
            switch (event.msg_event) {
                case AppEventType::BTN_SHORT_PRESS: {
                    ESP_LOGI(TAG, "Received: BTN_SHORT_PRESS");

                    ScreenAction action = self->current_screen_->on_short_press();
                    self->handle_screen_action(action);
                    break;
                }
                case AppEventType::BTN_LONG_PRESS: {
                    ESP_LOGI(TAG, "Received: BTN_LONG_PRESS");

                    lvgl_port_lock(portMAX_DELAY);
                        // self->next_screen();
                        self->current_screen_->on_long_press();
                    lvgl_port_unlock();
                    // app_wifi.connect();
                    break;
                }
                case AppEventType::TIME_SYNC_STATUS: {
                    ESP_LOGI(TAG, "Received: TIME_SYNC_STATUS");

                    int64_t t = time(NULL);
                    ESP_LOGI(TAG, "TIME_SYNC_STATUS: %lld", t);
                    break;
                }
                default: break;
            }
        }
    }
}

void AppController::handle_screen_action(ScreenAction action)
{
    switch (action) {
        case ScreenAction::INCREMENT_FEEDS: {
            ESP_LOGI(TAG, "handle_screen_action: INCREMENT_FEEDS");

            counter++;
            lvgl_port_lock(portMAX_DELAY);
                main_screen_.update_count(counter);
            lvgl_port_unlock();
            break;
        }
        case ScreenAction::OPTION_1_ACTION: {
            ESP_LOGI(TAG, "handle_screen_action: OPTION_1_ACTION");

            settings_.landscape_orientation = !settings_.landscape_orientation;
            lvgl_port_lock(portMAX_DELAY);
                options_screen_.update_settings(settings_);
            lvgl_port_unlock();
            break;
        }
        case ScreenAction::OPTION_2_ACTION: {
            ESP_LOGI(TAG, "handle_screen_action: OPTION_2_ACTION");

            settings_.feed_interval = (settings_.feed_interval + 1) % 10;
            if (settings_.feed_interval <= 1) {
                settings_.feed_interval = 1;
            }

            lvgl_port_lock(portMAX_DELAY);
                options_screen_.update_settings(settings_);
            lvgl_port_unlock();
            break;
        }
        case ScreenAction::OPTION_3_ACTION: {
            ESP_LOGI(TAG, "handle_screen_action: OPTION_3_ACTION");

            switch (settings_.animal) {
                case Animals::CAT: {
                    settings_.animal = Animals::DOG;
                    break;
                }
                case Animals::DOG: {
                    settings_.animal = Animals::HUMAN;
                    break;
                }
                case Animals::HUMAN: {
                    settings_.animal = Animals::CAT;
                    break;
                }
            }

            lvgl_port_lock(portMAX_DELAY);
                options_screen_.update_settings(settings_);
            lvgl_port_unlock();
            break;
        }
        default: break;
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

void AppController::next_screen()
{
    static uint8_t index = 0;
    index = (index + 1) % 2;
    current_screen_ = screens[index];
    lvgl_port_lock(portMAX_DELAY);
        current_screen_->show();
    lvgl_port_unlock();
}
