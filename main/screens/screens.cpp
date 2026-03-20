#include "screens.h"
#include "lvgl.h"

static lv_obj_t* scr;
static lv_obj_t* label_count;

void main_screen_init(void)
{
    scr = lv_obj_create(NULL);
    label_count = lv_label_create(scr);
    lv_obj_align(label_count, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(label_count, "0");
}

void main_screen_enter(void)
{
    lv_scr_load(scr);
}

void main_screen_render(int count)
{
    lv_label_set_text_fmt(label_count, "%d", count);
}

screen_t main_screen = {
    .enter = main_screen_enter,
    .render = main_screen_render,
};
