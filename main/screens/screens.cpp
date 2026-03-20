#include "screens.h"
#include "lvgl.h"

static lv_obj_t* main_scr;
static lv_obj_t* main_label_count;

static lv_obj_t* statistics_scr;
static lv_obj_t* statistics_label_stat;

void main_screen_init(void)
{
    main_scr = lv_obj_create(NULL);
    main_label_count = lv_label_create(main_scr);
    lv_obj_set_style_text_font(main_label_count, &lv_font_montserrat_22, 0);
    lv_obj_align(main_label_count, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(main_label_count, "0");
}

void main_screen_enter(void)
{
    lv_scr_load(main_scr);
}

void main_screen_render(int count)
{
    lv_label_set_text_fmt(main_label_count, "%d", count);
}

screen_t main_screen = {
    .enter = main_screen_enter,
    .render = main_screen_render,
};

void statistics_screen_init(void)
{
    statistics_scr = lv_obj_create(NULL);
    statistics_label_stat = lv_label_create(statistics_scr);
    lv_obj_set_style_text_font(statistics_label_stat, &lv_font_montserrat_22, 0);
    lv_obj_align(statistics_label_stat, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(statistics_label_stat, "Stats coming soon...");
}

void statistics_screen_enter(void)
{
    lv_scr_load(statistics_scr);
}

void statistics_screen_render(int count)
{
    lv_label_set_text_fmt(statistics_label_stat, "%s", "Stats coming soon...");
}

screen_t statistics_screen = {
    .enter = statistics_screen_enter,
    .render = statistics_screen_render,
};

