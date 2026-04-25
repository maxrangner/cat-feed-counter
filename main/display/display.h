#ifndef DISPLAY_H
#define DISPLAY_H

#include <cstdint>

#include "esp_lvgl_port.h"
#include "lvgl.h"

void display_init();
void display_set_brightness(uint8_t percent);
void display_set_rotation(lv_display_rotation_t rotation);

#endif
