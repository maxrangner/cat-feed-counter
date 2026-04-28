#include "app_controller.h"

#include <ctime>
#include <inttypes.h>
#include "lvgl.h"
#include "esp_log.h"
#include "esp_pm.h"

#include "display.h"
#include "config.h"


static const char *TAG = "app_controller";
static void btn_cb(button_event_t btn_event, uint8_t gpio_num, void* user_data);

static settings_t default_settings()
{
    settings_t settings = {
        .brightness = kBrightnessMedium,
        .half_feed_steps = false,
        .feed_interval = 1,
        .display_rotation = LV_DISPLAY_ROTATION_180,
        .day_reset_offset = 3,
    };

    return settings;
}

static void clamp_settings(settings_t* settings)
{
    if (
        settings->brightness != kBrightnessLow &&
        settings->brightness != kBrightnessMedium &&
        settings->brightness != kBrightnessHigh
    ) {
        settings->brightness = kBrightnessMedium;
    }

    if (settings->feed_interval < 1 || settings->feed_interval > 9) {
        settings->feed_interval = 1;
    }

    switch (settings->display_rotation) {
        case LV_DISPLAY_ROTATION_0:
        case LV_DISPLAY_ROTATION_90:
        case LV_DISPLAY_ROTATION_180:
        case LV_DISPLAY_ROTATION_270:
            break;
        default:
            settings->display_rotation = LV_DISPLAY_ROTATION_180;
            break;
    }

    if (settings->day_reset_offset > 4) {
        settings->day_reset_offset = 3;
    }
}

void AppController::reset_day_timer_cb(void* arg)
{
    auto* self = static_cast<AppController*>(arg);

    app_event_t event = {};
    event.msg_event = AppEventType::RESET_DAY;
    xQueueSend(self->in_queue_, &event, 0);
}

AppController::AppController() {}

bool AppController::save_today_to_flash()
{
    if (app_storage_.write_stats(&app_state_.today) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to save current day to flash");
        return false;
    }

    app_state_.stats.tot_num_feeds += app_state_.today.num_feeds;
    return true;
}

void AppController::update_current_screen_ui()
{
    lvgl_port_lock(portMAX_DELAY);
        screens_[current_screen_index_]->update_ui_state(app_state_);
    lvgl_port_unlock();
}

void AppController::load_current_screen()
{
    lvgl_port_lock(portMAX_DELAY);
        screens_[current_screen_index_]->show();
    lvgl_port_unlock();
}

void AppController::init()
{
    display_init();
    button_service_init();
    app_storage_.init();

    app_state_.settings = default_settings();
    app_storage_.load_settings(&app_state_.settings);
    clamp_settings(&app_state_.settings);
    app_state_.timer_running = false;
    app_state_.stats.tot_num_feeds = 0;
    app_state_.today.num_feeds = 0;
    app_state_.last_feed_time = 0;

    current_screen_index_ = 0;

    lvgl_port_lock(portMAX_DELAY);
        display_set_rotation(app_state_.settings.display_rotation);
    lvgl_port_unlock();

    in_queue_ = xQueueCreate(10, sizeof(app_event_t));

    esp_timer_create_args_t timer_args = {};
    timer_args.callback = &reset_day_timer_cb;
    timer_args.arg = this;
    timer_args.dispatch_method = ESP_TIMER_TASK;
    timer_args.name = "reset_day_timer";
    esp_timer_create(&timer_args, &reset_day_timer);

    lvgl_port_lock(portMAX_DELAY);
        main_screen_.init();
        options_screen_.init();
        stats_screen_.init();
    lvgl_port_unlock();

    xTaskCreate(
        app_task,
        "appTask",
        8192,
        this,
        1,
        &task_app_controller_
    );

    button_cfg_t btn_main_cfg = {
        .gpio_num = kBtnMainPin,
        .hasPullup = true,
        .debounce = 25,
        .long_press_dur = 500,
        .btn_callback = btn_cb,
        .user_data = this,
    };
    button_init(&btn_main_cfg, &btn_main_);

    button_cfg_t btn_side_cfg = {
        .gpio_num = kBtnSidePin,
        .hasPullup = true,
        .debounce = 25,
        .long_press_dur = 500,
        .btn_callback = btn_cb,
        .user_data = this,
    };
    button_init(&btn_side_cfg, &btn_side_);
}

void AppController::app_task(void* pvParameters)
{
    auto* self = static_cast<AppController*>(pvParameters);

    self->app_wifi_.init(self->getAppQueue());
    self->app_wifi_.sync_time();
    self->app_storage_.load_stats(&self->app_state_.stats);

    app_event_t event = {};

    self->load_current_screen();
    self->update_current_screen_ui();

    display_set_brightness(self->app_state_.settings.brightness);

    while(1) {
        if (xQueueReceive(self->in_queue_, &event, pdMS_TO_TICKS(60000))) { // 60000 == One minute
            self->handle_app_events(event);
        }
        self->update_current_screen_ui();
    }
}

void AppController::handle_app_events(app_event_t event)
{
    ScreenAction action = ScreenAction::NONE;

    switch (event.msg_event) {
        case AppEventType::BTN_MAIN_SHORT_PRESS: action = screens_[current_screen_index_]->primary_action(); break;
        case AppEventType::BTN_MAIN_LONG_PRESS: next_screen(); break;
        case AppEventType::BTN_SIDE_SHORT_PRESS: action = screens_[current_screen_index_]->secondary_action(); break;
        case AppEventType::BTN_SIDE_LONG_PRESS: ESP_LOGI(TAG, "BTN_SIDE_LONG_PRESS"); break;
        case AppEventType::TIME_SYNCED: set_reset_timer(); break;
        case AppEventType::RESET_DAY: reset_day(); break;
        default: break;
    }

    switch (action) {
        case ScreenAction::INCREMENT_FEEDS: increment_count(); break;
        case ScreenAction::CHANGE_BRIGHTNESS: change_brightness(); break;
        case ScreenAction::INCREMENT_FEED_INTERVAL: increment_feed_interval(); break;
        case ScreenAction::ROTATE_DISPLAY: rotate_display(); break;
        case ScreenAction::RESET_OFFSET: change_reset_offset(); break;
        default: break;
    }
}

void AppController::post_event(app_event_t event)
{
    xQueueSend(in_queue_, &event, 0);
}

QueueHandle_t AppController::getAppQueue()
{
    return in_queue_;
}

void AppController::next_screen()
{
    ESP_LOGI(TAG, "next_screen()");

    current_screen_index_ = (current_screen_index_ + 1) % kNumScreens;
    load_current_screen();
    update_current_screen_ui();
}

void AppController::set_reset_timer()
{
    ESP_LOGI(TAG, "set_reset_timer()");

    if (app_state_.timer_running) {
        esp_timer_stop(reset_day_timer);
    }

    time_t now = time(NULL);
    tm* reset_time = localtime(&now);
    reset_time->tm_hour = app_state_.settings.day_reset_offset;
    reset_time->tm_min = 0;
    reset_time->tm_sec = 0;

    time_t target = mktime(reset_time);

    if (target <= now) {
        reset_time->tm_mday += 1;
        target = mktime(reset_time);
    }

    uint64_t next_reset = (target - now) * 1000000ULL;

    esp_timer_start_once(reset_day_timer, next_reset);
    ESP_LOGI(TAG, "Next reset in %" PRIu64 " seconds", next_reset / 1000000ULL);
    app_state_.timer_running = true;
}

void AppController::reset_day()
{
    ESP_LOGI(TAG, "reset_day()");
    
    app_state_.timer_running = false;

    if (save_today_to_flash()) {
        app_state_.today.num_feeds = 0;
        app_state_.last_feed_time = 0;
    }

    set_reset_timer(); 
}

void AppController::increment_count()
{
    ESP_LOGI(TAG, "increment_count()");

    app_state_.today.num_feeds++;
    app_state_.last_feed_time = time(NULL);

    update_current_screen_ui();
}

void AppController::change_brightness()
{
    ESP_LOGI(TAG, "change_brightness()");

    if (app_state_.settings.brightness == kBrightnessLow) {
        app_state_.settings.brightness = kBrightnessMedium;
    } else if (app_state_.settings.brightness == kBrightnessMedium) {
        app_state_.settings.brightness = kBrightnessHigh;
    } else {
        app_state_.settings.brightness = kBrightnessLow;
    }
    
    update_current_screen_ui();
    display_set_brightness(app_state_.settings.brightness);
    app_storage_.save_settings(&app_state_.settings);
}

void AppController::increment_feed_interval()
{
    ESP_LOGI(TAG, "increment_feed_interval()");

    app_state_.settings.feed_interval = (app_state_.settings.feed_interval + 1) % 10;
    if (app_state_.settings.feed_interval <= 1) {
        app_state_.settings.feed_interval = 1;
    }

    update_current_screen_ui();
    app_storage_.save_settings(&app_state_.settings);
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
        display_set_rotation(app_state_.settings.display_rotation);
    lvgl_port_unlock();
    update_current_screen_ui();
    app_storage_.save_settings(&app_state_.settings);
}

void AppController::change_reset_offset()
{
    ESP_LOGI(TAG, "change_reset_offset()");

    app_state_.settings.day_reset_offset = (app_state_.settings.day_reset_offset + 1) % 5;

    update_current_screen_ui();

    set_reset_timer();
    app_storage_.save_settings(&app_state_.settings);
}

void btn_cb(button_event_t btn_event, uint8_t gpio_num, void* user_data)
{
    AppController *self = (AppController *)user_data;

    
    app_event_t event = {};
    event.msg_event = AppEventType::NONE;

    if (gpio_num == kBtnMainPin) {
        if (btn_event == BTN_SHORT_PRESS) event.msg_event = AppEventType::BTN_MAIN_SHORT_PRESS;
        if (btn_event == BTN_LONG_PRESS) event.msg_event = AppEventType::BTN_MAIN_LONG_PRESS;
    }
    if (gpio_num == kBtnSidePin) {
        if (btn_event == BTN_SHORT_PRESS) event.msg_event = AppEventType::BTN_SIDE_SHORT_PRESS;
        if (btn_event == BTN_LONG_PRESS) event.msg_event = AppEventType::BTN_SIDE_LONG_PRESS;
    }
    if (event.msg_event != AppEventType::NONE) {
        self->post_event(event);
    }
}
