#ifndef TYPES_H
#define TYPES_H

#include "app_wifi.h"

enum class AppEventType {
    BTN_SHORT_PRESS,
    BTN_LONG_PRESS,
    WIFI_UPDATE,
};

typedef struct {
    AppEventType msg_event;
    union {
        WifiState wifi_state;
    };
    
} app_event;

#endif