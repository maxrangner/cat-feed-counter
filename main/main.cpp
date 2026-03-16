#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "display.h"
#include "system_manager.h"
#include "ui_manager.h"

extern "C" void app_main(void)
{
    display_init();
    SystemManager sys_mgr;
    UiManager ui_mgr;

    sys_mgr.init();
    ui_mgr.init();
    sys_mgr.connectUi(ui_mgr.getInputQueue());
    ui_mgr.connectSystem(sys_mgr.getInputQueue());
    
    uint8_t count = 0;

    lvgl_port_lock(portMAX_DELAY);
        lv_obj_set_style_bg_color(lv_scr_act(), lv_color_white(), 0);

        lv_obj_t *label = lv_label_create(lv_scr_act());
        lv_obj_set_style_text_font(label, &lv_font_montserrat_22, 0);
        lv_obj_set_style_text_color(label, lv_color_black(), 0);
        lv_label_set_long_mode(label, LV_LABEL_LONG_CLIP);
        lv_label_set_text(label, "Count: 0");
        lv_obj_set_pos(label, 40, LCD_V_RES / 2);

    lvgl_port_unlock();

    while (1)
    {
        display_feeds(label, &count);
        count++;
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}