#ifndef TYPES_H
#define TYPES_H

#include "app_wifi.h"

#include "lvgl.h"

enum class AppEventType {
    BTN_SHORT_PRESS,
    BTN_LONG_PRESS,
    TIME_SYNCED,
};

typedef struct {
    AppEventType msg_event;
    union {
        WifiState wifi_state;
    };
} app_event_t;

typedef struct {
    bool landscape_orientation;
    uint8_t brightness;
    bool half_feed_steps;
    uint8_t feed_interval;
    lv_display_rotation_t display_rotation;
} settings_t;

typedef struct {
    uint32_t num_feeds;
} day_data_t;

typedef struct {
    uint32_t tot_num_feeds;
} stats_t;

#endif