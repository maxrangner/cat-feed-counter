#ifndef TYPES_H
#define TYPES_H

#include <ctime>

#include "lvgl.h"

enum class AppEventType {
    NONE,
    BTN_MAIN_SHORT_PRESS,
    BTN_MAIN_LONG_PRESS,
    BTN_SIDE_SHORT_PRESS,
    BTN_SIDE_LONG_PRESS,
    TIME_SYNCED,
    RESET_DAY,
};

struct app_event_t {
    AppEventType msg_event;
};

struct settings_t {
    uint8_t brightness;
    bool half_feed_steps;
    uint8_t feed_interval;
    lv_display_rotation_t display_rotation;
    uint8_t day_reset_offset;
};

struct day_data_t {
    uint32_t num_feeds;
};

struct stats_t {
    uint32_t tot_num_feeds;
};

struct app_state_t {
    settings_t settings;
    bool timer_running;
    stats_t stats;
    day_data_t today;
    time_t last_feed_time;
};

#endif
