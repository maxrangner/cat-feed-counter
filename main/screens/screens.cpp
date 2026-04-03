#include "screens.h"
#include "lvgl.h"

static lv_obj_t* main_scr;
static lv_obj_t* main_label_text;
static lv_obj_t* main_label_count;
static lv_obj_t* main_label_bar;

static lv_obj_t* statistics_scr;
static lv_obj_t* statistics_label_stat;

void main_screen_init()
{
    main_scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(main_scr, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(main_scr, LV_OPA_COVER, 0);

    main_label_text = lv_label_create(main_scr);
    lv_label_set_long_mode(main_label_text, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(main_label_text, 150);
    lv_obj_set_style_text_font(main_label_text, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(main_label_text, lv_color_black(), 0);
    lv_obj_align(main_label_text, LV_ALIGN_LEFT_MID, 20, -15);
    lv_label_set_text(main_label_text, "Todays number of feedings");

    main_label_count = lv_label_create(main_scr);
    lv_obj_set_style_text_font(main_label_count, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(main_label_text, lv_color_black(), 0);
    lv_obj_align(main_label_count, LV_ALIGN_CENTER, 50, -15);
    lv_label_set_text(main_label_count, "0");

    main_label_bar = lv_bar_create(main_scr);
    lv_obj_set_style_bg_color(main_label_bar, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(main_label_bar, lv_color_black(), 0);
    lv_obj_align(main_label_bar, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_set_size(main_label_bar, 150, 20);
    lv_bar_set_range(main_label_bar, 0, 10);
    lv_bar_set_value(main_label_bar, 10, LV_ANIM_OFF);
}

void main_screen_enter()
{
    lv_screen_load(main_scr);
}

void main_screen_render(int count)
{
    lv_label_set_text_fmt(main_label_count, "%d", count);
    lv_bar_set_value(main_label_bar, count, LV_ANIM_OFF);
}

screen_t main_screen = {
    .enter = main_screen_enter,
    .render = main_screen_render,
};

void statistics_screen_init()
{
    statistics_scr = lv_obj_create(NULL);
    statistics_label_stat = lv_label_create(statistics_scr);
    lv_obj_set_style_text_font(statistics_label_stat, &lv_font_montserrat_24, 0);
    lv_obj_align(statistics_label_stat, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(statistics_label_stat, "Stats coming soon...");
}

void statistics_screen_enter()
{
    lv_screen_load(statistics_scr);
}

void statistics_screen_render(int count)
{
    lv_label_set_text(statistics_label_stat, "Stats coming soon...");
}

screen_t statistics_screen = {
    .enter = statistics_screen_enter,
    .render = statistics_screen_render,
};

