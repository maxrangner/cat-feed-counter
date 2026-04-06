#ifndef TYPES_H
#define TYPES_H

#include "app_wifi.h"

enum class AppEventType {
    BTN_SHORT_PRESS,
    BTN_LONG_PRESS,
    TIME_SYNC_STATUS,
    DISPLAY_UPDATE,
};

typedef struct {
    AppEventType msg_event;
    union {
        WifiState wifi_state;
    };
    
} app_event_t;

typedef struct {
    uint32_t num_feeds;
} day_data_t;

typedef struct {
    uint32_t tot_num_feeds;
} stats_t;

#endif