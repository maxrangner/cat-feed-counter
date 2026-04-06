#include "screens.h"

/*
-------------------------------MAIN SCREEN-------------------------------
*/

void MainScreen::init()
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

void MainScreen::enter()
{
    lv_screen_load(main_scr);
}

void MainScreen::render()
{
    uint8_t count = 0;
    lv_label_set_text_fmt(main_label_count, "%d", count);
    lv_bar_set_value(main_label_bar, count, LV_ANIM_OFF);
}

void MainScreen::on_short_press() {

}

void MainScreen::on_long_press() {

}

/*
-------------------------------OPTIONS SCREEN-------------------------------
*/

void OptionsScreen::init()
{
    options_scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(options_scr, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(options_scr, LV_OPA_COVER, 0);

    options_label_1_text = lv_label_create(options_scr);
    lv_label_set_long_mode(options_label_1_text, LV_LABEL_LONG_CLIP);
    lv_obj_set_width(options_label_1_text, 150);
    lv_obj_set_style_text_font(options_label_1_text, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(options_label_1_text, lv_color_black(), 0);
    lv_obj_align(options_label_1_text, LV_ALIGN_LEFT_MID, 40, -15);
    lv_label_set_text(options_label_1_text, "Option 1");

    options_variable_1_text = lv_label_create(options_scr);
    lv_label_set_long_mode(options_label_1_text, LV_LABEL_LONG_CLIP);
    lv_obj_set_width(options_label_1_text, 150);
    lv_obj_set_style_text_font(options_label_1_text, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(options_label_1_text, lv_color_black(), 0);
    lv_obj_align(options_label_1_text, LV_ALIGN_LEFT_MID, 40, -15);
    lv_label_set_text(options_label_1_text, "Option 1");

    options_label_2_text = lv_label_create(options_scr);
    lv_label_set_long_mode(options_label_2_text, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(options_label_2_text, 150);
    lv_obj_set_style_text_font(options_label_2_text, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(options_label_2_text, lv_color_black(), 0);
    lv_obj_align(options_label_2_text, LV_ALIGN_LEFT_MID, 40, 15);
    lv_label_set_text(options_label_2_text, "Option 2");
}

void OptionsScreen::enter()
{
    lv_screen_load(options_scr);
}

void OptionsScreen::render()
{

}

void OptionsScreen::on_short_press() {

}

void OptionsScreen::on_long_press() {

}
