#include "app_controller.h"

#include <ctime>
#include <inttypes.h>
#include "lvgl.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_pm.h"

#include "display.h"


static const char *TAG = "app_controller";
static void btn_cb(button_event_t btn_event, void* user_data);

static void nvs_init()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

AppController::AppController() {}

void AppController::init()
{
    display_init();
    nvs_init();
    button_service_init();

    app_state_.settings = {
        .landscape_orientation = true,
        .brightness = 30,
        .half_feed_steps = false,
        .feed_interval = 3,
        .display_rotation = LV_DISPLAY_ROTATION_90,
    };
    app_state_.stats.tot_num_feeds = 0;
    app_state_.current_screen = screens[1];
    app_state_.today.num_feeds = 0;

    in_queue_ = xQueueCreate(10, sizeof(app_event_t));

    lvgl_port_lock(portMAX_DELAY);
        main_screen_.init();
        options_screen_.init();
        options_screen_.update_settings(app_state_.settings);
    lvgl_port_unlock();

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

    self->app_wifi_.init(self->getAppQueue());
    self->app_wifi_.sync_time();
    self->app_storage_.init();
    self->app_storage_.load_stats(&self->app_state_.stats);

    app_event_t event;
    
    lvgl_port_lock(portMAX_DELAY);
        self->main_screen_.update_count(self->app_state_.stats.tot_num_feeds);
        self->app_state_.current_screen->show();
    lvgl_port_unlock();

    display_set_brightness(30); // %

    while(1) {
        if (xQueueReceive(self->in_queue_, &event, portMAX_DELAY)) {
            self->handle_app_events(event);
        }
    }
}

void AppController::handle_app_events(app_event_t event)
{
    ScreenAction action = ScreenAction::NONE;

    switch (event.msg_event) {
        case AppEventType::BTN_SHORT_PRESS: action = app_state_.current_screen->on_short_press(); break;
        case AppEventType::BTN_LONG_PRESS: action = app_state_.current_screen->on_long_press(); break;
        case AppEventType::TIME_SYNCED: ESP_LOGI(TAG, "TIME_SYNCED: %lld", (uint64_t)time(NULL)); break;
        default: break;
    }

    switch (action) {
        case ScreenAction::INCREMENT_FEEDS: increment_count(); break;
        case ScreenAction::SAVE_DATA: save_data(); break;
        case ScreenAction::CHANGE_BRIGHTNESS: change_brightness(); break;
        case ScreenAction::INCREMENT_FEED_INTERVAL: increment_feed_interval(); break;
        case ScreenAction::ROTATE_DISPLAY: rotate_display(); break;
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
    if (btn_event == BTN_SHORT_PRESS) event.msg_event = AppEventType::BTN_SHORT_PRESS;
    if (btn_event == BTN_LONG_PRESS) event.msg_event = AppEventType::BTN_LONG_PRESS;
    self->post_event(event);
}

QueueHandle_t AppController::getAppQueue()
{
    return in_queue_;
}

void AppController::next_screen()
{
    ESP_LOGI(TAG, "next_screen()");

    static uint8_t index = 0;
    index = (index + 1) % 2;
    app_state_.current_screen = screens[index];
    lvgl_port_lock(portMAX_DELAY);
        app_state_.current_screen->show();
    lvgl_port_unlock();
}

void AppController::reset_day()
{
    ESP_LOGI(TAG, "reset_day()");

    app_state_.today.num_feeds = 0;
}

void AppController::save_data()
{
    ESP_LOGI(TAG, "save_data()");

    app_storage_.write_stats(&app_state_.today);
    reset_day();

    lvgl_port_lock(portMAX_DELAY);
        main_screen_.update_count(0);
    lvgl_port_unlock();
}

void AppController::increment_count()
{
    ESP_LOGI(TAG, "increment_count()");
    
    app_state_.today.num_feeds++;

    lvgl_port_lock(portMAX_DELAY);
        main_screen_.update_count(app_state_.today.num_feeds);
    lvgl_port_unlock();
}

void AppController::change_brightness()
{
    ESP_LOGI(TAG, "change_brightness()");

    app_state_.settings.brightness += 10;
    if (app_state_.settings.brightness > 70) {
        app_state_.settings.brightness = 10;
    }
    
    lvgl_port_lock(portMAX_DELAY);
        options_screen_.update_settings(app_state_.settings);
    lvgl_port_unlock();
    
    display_set_brightness(app_state_.settings.brightness);
}

void AppController::increment_feed_interval()
{
    ESP_LOGI(TAG, "increment_feed_interval()");

    app_state_.settings.feed_interval = (app_state_.settings.feed_interval + 1) % 10;
    if (app_state_.settings.feed_interval <= 1) {
        app_state_.settings.feed_interval = 1;
    }

    lvgl_port_lock(portMAX_DELAY);
        options_screen_.update_settings(app_state_.settings);
    lvgl_port_unlock();
}

void AppController::rotate_display()
{
    ESP_LOGI(TAG, "rotate_display()");

    switch (app_state_.settings.display_rotation) {
        case LV_DISPLAY_ROTATION_0: app_state_.settings.display_rotation = LV_DISPLAY_ROTATION_90; break;
        case LV_DISPLAY_ROTATION_90: app_state_.settings.display_rotation = LV_DISPLAY_ROTATION_180; break;
        case LV_DISPLAY_ROTATION_180: app_state_.settings.display_rotation = LV_DISPLAY_ROTATION_270; break;
        case LV_DISPLAY_ROTATION_270:
        default: app_state_.settings.display_rotation = LV_DISPLAY_ROTATION_0; break;
    }

    lvgl_port_lock(portMAX_DELAY);
        options_screen_.update_settings(app_state_.settings);
        display_set_rotation(app_state_.settings.display_rotation);
    lvgl_port_unlock();

}