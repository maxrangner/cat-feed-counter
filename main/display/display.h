#ifndef DISPLAY_H
#define DISPLAY_H

#include "esp_lvgl_port.h"
#include "lvgl.h"

// =============================================================================
// Pin definitions — Waveshare ESP32-C6-LCD-1.47
// =============================================================================

#define PIN_NUM_MOSI 6
#define PIN_NUM_CLK  7
#define PIN_NUM_CS   14
#define PIN_NUM_DC   15
#define PIN_NUM_RST  21
#define PIN_NUM_BL   22

#define LCD_H_RES    172
#define LCD_V_RES    320
#define LCD_BUF_SIZE (LCD_H_RES * 80)

void display_init(void);
void display_feeds(lv_obj_t* label, uint8_t* count);

#endif