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
    lv_label_set_text_fmt(main_label_count, "%d", counter_);

    main_label_bar = lv_bar_create(main_scr);
    lv_obj_set_style_bg_color(main_label_bar, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(main_label_bar, lv_color_black(), 0);
    lv_obj_align(main_label_bar, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_set_size(main_label_bar, 150, 20);
    lv_bar_set_range(main_label_bar, 0, 10);
    lv_bar_set_value(main_label_bar, counter_, LV_ANIM_ON);

    counter_ = 0;
}

void MainScreen::show()
{
    lv_screen_load(main_scr);
}

void MainScreen::update()
{
    lv_label_set_text_fmt(main_label_count, "%d", counter_);
    lv_bar_set_value(main_label_bar, counter_, LV_ANIM_ON);
}

ScreenAction MainScreen::on_short_press()
{
    return ScreenAction::INCREMENT_FEEDS;
}

ScreenAction MainScreen::on_long_press()
{
    return ScreenAction::SAVE_DATA;
}

void MainScreen::update_count(uint8_t count)
{
    counter_ = count;
    update();
}

/*
-------------------------------OPTIONS SCREEN-------------------------------
*/

void OptionsScreen::init()
{
    options_scr_ = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(options_scr_, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(options_scr_, LV_OPA_COVER, 0);

    brightness_title_label_ = lv_label_create(options_scr_);
    lv_label_set_long_mode(brightness_title_label_, LV_LABEL_LONG_CLIP);
    lv_obj_set_style_text_font(brightness_title_label_, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(brightness_title_label_, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_align(brightness_title_label_, LV_ALIGN_LEFT_MID, 30, -35);
    lv_label_set_text(brightness_title_label_, "Brightness:");

    brightness_value_label_ = lv_label_create(options_scr_);
    lv_label_set_long_mode(brightness_value_label_, LV_LABEL_LONG_CLIP);
    lv_obj_set_style_text_font(brightness_value_label_, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(brightness_value_label_, lv_color_black(), 0);
    lv_obj_align(brightness_value_label_, LV_ALIGN_LEFT_MID, 210, -35);

    feed_interval_title_label_ = lv_label_create(options_scr_);
    lv_label_set_long_mode(feed_interval_title_label_, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_font(feed_interval_title_label_, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(feed_interval_title_label_, lv_color_black(), 0);
    lv_obj_align(feed_interval_title_label_, LV_ALIGN_LEFT_MID, 30, 0);
    lv_label_set_text(feed_interval_title_label_, "Feed interval:");

    feed_interval_value_label_ = lv_label_create(options_scr_);
    lv_label_set_long_mode(feed_interval_value_label_, LV_LABEL_LONG_CLIP);
    lv_obj_set_style_text_font(feed_interval_value_label_, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(feed_interval_value_label_, lv_color_black(), 0);
    lv_obj_align(feed_interval_value_label_, LV_ALIGN_LEFT_MID, 210, 0);

    screen_orientation_title_label_ = lv_label_create(options_scr_);
    lv_label_set_long_mode(screen_orientation_title_label_, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_font(screen_orientation_title_label_, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(screen_orientation_title_label_, lv_color_black(), 0);
    lv_obj_align(screen_orientation_title_label_, LV_ALIGN_LEFT_MID, 30, 35);
    lv_label_set_text(screen_orientation_title_label_, "Rotate display:");

    screen_orientation_value_label_ = lv_label_create(options_scr_);
    lv_label_set_long_mode(screen_orientation_value_label_, LV_LABEL_LONG_CLIP);
    lv_obj_set_style_text_font(screen_orientation_value_label_, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(screen_orientation_value_label_, lv_color_black(), 0);
    lv_obj_align(screen_orientation_value_label_, LV_ALIGN_LEFT_MID, 210, 35);

    option_brightness_.label = brightness_title_label_;
    option_brightness_.label_state = brightness_value_label_;
    option_brightness_.action = ScreenAction::CHANGE_BRIGHTNESS;

    option_feed_interval_.label = feed_interval_title_label_;
    option_feed_interval_.label_state = feed_interval_value_label_;
    option_feed_interval_.action = ScreenAction::INCREMENT_FEED_INTERVAL;

    option_screen_orientation_.label = screen_orientation_title_label_;
    option_screen_orientation_.label_state = screen_orientation_value_label_;
    option_screen_orientation_.action = ScreenAction::ROTATE_DISPLAY;
}

void OptionsScreen::show()
{
    lv_screen_load(options_scr_);
}

void OptionsScreen::update()
{
    for (uint8_t i = 0; i < NUM_OPTIONS; i++) {
        if (i == selected_index_) {
            lv_obj_set_style_text_color(options_[i]->label, lv_palette_main(LV_PALETTE_RED), 0);
        } else {
            lv_obj_set_style_text_color(options_[i]->label, lv_color_black(), 0); 
        }
    }
    lv_label_set_text_fmt(brightness_value_label_, "%d", settings_.brightness);
    lv_label_set_text_fmt(feed_interval_value_label_, "%d hrs", settings_.feed_interval);
    lv_label_set_text_fmt(screen_orientation_value_label_, "%s", (settings_.landscape_orientation ? "TRUE" : "FALSE"));

    switch (settings_.display_rotation) {
        case LV_DISPLAY_ROTATION_0: lv_label_set_text(screen_orientation_value_label_, "0"); break;
        case LV_DISPLAY_ROTATION_90: lv_label_set_text(screen_orientation_value_label_, "90"); break;
        case LV_DISPLAY_ROTATION_180: lv_label_set_text(screen_orientation_value_label_, "180"); break;
        case LV_DISPLAY_ROTATION_270: lv_label_set_text(screen_orientation_value_label_, "270"); break;
    }
}

ScreenAction OptionsScreen::on_short_press()
{
    ScreenAction action = options_[selected_index_]->action;
    return action;
}

ScreenAction OptionsScreen::on_long_press()
{
    uint8_t index = (++selected_index_) % NUM_OPTIONS;
    selected_index_ = index;
    update();
    return ScreenAction::NONE;
}

void OptionsScreen::update_settings(settings_t settings)
{
    settings_ = settings;
    update();
}
