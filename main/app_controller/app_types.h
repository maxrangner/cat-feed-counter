#ifndef TYPES_H
#define TYPES_H

#include "app_wifi.h"

#include "lvgl.h"

class Screen;

enum class AppEventType {
    BTN_SHORT_PRESS,
    BTN_LONG_PRESS,
    TIME_SYNCED,
    RESET_DAY,
};

typedef struct {
    AppEventType msg_event;
    union {
        WifiState wifi_state;
    };
} app_event_t;

typedef struct {
    uint8_t brightness;
    bool half_feed_steps;
    uint8_t feed_interval;
    lv_display_rotation_t display_rotation;
    uint8_t day_reset_offset;
} settings_t;

typedef struct {
    uint32_t num_feeds;
    uint32_t last_feed_time;
} day_data_t;

typedef struct {
    uint32_t tot_num_feeds;
} stats_t;

typedef struct {
    settings_t settings;
    bool timer_running;
    stats_t stats;
    Screen* current_screen;
    day_data_t today;
    uint32_t next_feed_time;
} app_state_t;

#endif