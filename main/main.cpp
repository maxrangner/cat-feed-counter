#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "display.h"

extern "C" void app_main(void)
{
    display_init();
    uint8_t count = 0;

    lvgl_port_lock(portMAX_DELAY);
        lv_obj_set_style_bg_color(lv_scr_act(), lv_color_white(), 0);
        lv_obj_t *label = lv_label_create(lv_scr_act());
        lv_obj_set_style_text_font(label, &lv_font_montserrat_22, 0);
        lv_obj_set_style_text_color(label, lv_color_black(), 0);
        lv_label_set_text(label, "Count: 0");
        lv_obj_center(label);
    lvgl_port_unlock();

    while (true)
    {
        display_feeds(label, &count);
        count++;
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}