#include "screens.h"

void Screen::update_ui_state(const app_state_t& state)
{
    ui_state_.brightness = state.settings.brightness;
    ui_state_.feed_interval = state.settings.feed_interval;
    ui_state_.half_feed_steps = state.settings.half_feed_steps;
    ui_state_.screen_orientation = state.settings.display_rotation;
    ui_state_.count = state.today.num_feeds;
    ui_state_.reset_day_offset = state.settings.day_reset_offset;
    ui_state_.last_feed_time = state.last_feed_time;
    
    apply_layout();
    update();
}

/*
-------------------------------MAIN SCREEN-------------------------------
*/

void MainScreen::apply_layout()
{
    if (ui_state_.screen_orientation == LV_DISPLAY_ROTATION_90 || ui_state_.screen_orientation == LV_DISPLAY_ROTATION_270) {
        lv_obj_set_style_text_font(main_label_text_, &lv_font_montserrat_24, 0);
        lv_label_set_long_mode(main_label_text_, LV_LABEL_LONG_CLIP);
        lv_obj_align(main_label_text_, LV_ALIGN_LEFT_MID, 20, -35);

        lv_obj_set_style_text_font(main_label_time_, &lv_font_montserrat_24, 0);
        lv_label_set_long_mode(main_label_time_, LV_LABEL_LONG_CLIP);
        lv_obj_align(main_label_time_, LV_ALIGN_LEFT_MID, 20, 0);

        lv_obj_set_style_text_font(main_label_count_, &lv_font_montserrat_48, 0);
        lv_obj_align(main_label_count_, LV_ALIGN_CENTER, 100, -15);
        lv_obj_align(main_label_bar_, LV_ALIGN_BOTTOM_MID, 0, -20);
    } else {
        lv_obj_set_style_text_font(main_label_text_, &lv_font_montserrat_24, 0);
        lv_label_set_long_mode(main_label_text_, LV_LABEL_LONG_WRAP);
        lv_obj_align(main_label_text_, LV_ALIGN_CENTER, 0, -80);

        lv_obj_set_style_text_font(main_label_time_, &lv_font_montserrat_24, 0);
        lv_label_set_long_mode(main_label_time_, LV_LABEL_LONG_WRAP);
        lv_obj_align(main_label_time_, LV_ALIGN_CENTER, 0, -50);

        lv_obj_set_style_text_font(main_label_count_, &lv_font_montserrat_48, 0);
        lv_obj_align(main_label_count_, LV_ALIGN_CENTER, 0, 30);
        lv_obj_align(main_label_bar_, LV_ALIGN_CENTER, 0, 80);
    }
}

void MainScreen::init()
{
    main_scr_ = lv_obj_create(NULL);

    lv_obj_set_style_bg_color(main_scr_, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(main_scr_, LV_OPA_COVER, 0);

    main_label_text_ = lv_label_create(main_scr_);
    lv_obj_set_style_text_color(main_label_text_, lv_color_black(), 0);
    lv_label_set_text(main_label_text_, "Last feeding");

    main_label_time_ = lv_label_create(main_scr_);
    lv_obj_set_style_text_color(main_label_time_, lv_color_black(), 0);
    lv_label_set_text(main_label_time_, "--:--");

    main_label_count_ = lv_label_create(main_scr_);
    lv_obj_set_style_text_color(main_label_text_, lv_color_black(), 0);
    lv_label_set_text_fmt(main_label_count_, "%d", ui_state_.count);

    main_label_bar_ = lv_bar_create(main_scr_);
    lv_obj_set_style_bg_color(main_label_bar_, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(main_label_bar_, lv_color_black(), 0);
    lv_obj_set_size(main_label_bar_, 150, 20);
    lv_bar_set_range(main_label_bar_, 0, 10);
    lv_bar_set_value(main_label_bar_, ui_state_.count, LV_ANIM_ON);

    apply_layout();
}

void MainScreen::show()
{
    lv_screen_load(main_scr_);
}

void MainScreen::update()
{
    lv_label_set_text_fmt(main_label_count_, "%d", ui_state_.count);
    
    if (ui_state_.last_feed_time == 0) {
        lv_label_set_text(main_label_time_, "--:--");
    } else {
        tm* last_feeding = localtime(&ui_state_.last_feed_time);
        lv_label_set_text_fmt(
            main_label_time_,
            "%02d:%02d",
            last_feeding->tm_hour,
            last_feeding->tm_min
        );
    }

    time_t total = ui_state_.feed_interval * 3600;
    time_t elapsed = time(NULL) - ui_state_.last_feed_time;
    lv_bar_set_range(main_label_bar_, 0, total);
    lv_bar_set_value(main_label_bar_, elapsed, LV_ANIM_ON);
}

ScreenAction MainScreen::primary_action()
{
    return ScreenAction::INCREMENT_FEEDS;
}

ScreenAction MainScreen::secondary_action()
{
    return ScreenAction::NONE;
}

/*
-------------------------------OPTIONS SCREEN-------------------------------
*/

void OptionsScreen::apply_layout()
{
    if (ui_state_.screen_orientation == LV_DISPLAY_ROTATION_90 || ui_state_.screen_orientation == LV_DISPLAY_ROTATION_270) {
        lv_obj_set_style_text_font(brightness_title_label_, &lv_font_montserrat_24, 0);
        lv_obj_set_style_text_font(brightness_value_label_, &lv_font_montserrat_24, 0);

        lv_obj_set_style_text_font(feed_interval_title_label_, &lv_font_montserrat_24, 0);
        lv_obj_set_style_text_font(feed_interval_value_label_, &lv_font_montserrat_24, 0);

        lv_obj_set_style_text_font(screen_orientation_title_label_, &lv_font_montserrat_24, 0);
        lv_obj_set_style_text_font(screen_orientation_value_label_, &lv_font_montserrat_24, 0);

        lv_obj_set_style_text_font(reset_offset_title_label_, &lv_font_montserrat_24, 0);
        lv_obj_set_style_text_font(reset_offset_value_label_, &lv_font_montserrat_24, 0);

        lv_obj_align(brightness_title_label_, LV_ALIGN_LEFT_MID, 20, -45);
        lv_obj_align(brightness_value_label_, LV_ALIGN_LEFT_MID, 215, -45);

        lv_obj_align(feed_interval_title_label_, LV_ALIGN_LEFT_MID, 20, -15);
        lv_obj_align(feed_interval_value_label_, LV_ALIGN_LEFT_MID, 215, -15);

        lv_obj_align(screen_orientation_title_label_, LV_ALIGN_LEFT_MID, 20, 15);
        lv_obj_align(screen_orientation_value_label_, LV_ALIGN_LEFT_MID, 215, 15);

        lv_obj_align(reset_offset_title_label_, LV_ALIGN_LEFT_MID, 20, 45);
        lv_obj_align(reset_offset_value_label_, LV_ALIGN_LEFT_MID, 215, 45);
    } else {
        lv_obj_set_style_text_font(brightness_title_label_, &lv_font_montserrat_20, 0);
        lv_obj_set_style_text_font(brightness_value_label_, &lv_font_montserrat_20, 0);

        lv_obj_set_style_text_font(feed_interval_title_label_, &lv_font_montserrat_20, 0);
        lv_obj_set_style_text_font(feed_interval_value_label_, &lv_font_montserrat_20, 0);

        lv_obj_set_style_text_font(screen_orientation_title_label_, &lv_font_montserrat_20, 0);
        lv_obj_set_style_text_font(screen_orientation_value_label_, &lv_font_montserrat_20, 0);

        lv_obj_set_style_text_font(reset_offset_title_label_, &lv_font_montserrat_20, 0);
        lv_obj_set_style_text_font(reset_offset_value_label_, &lv_font_montserrat_20, 0);

        lv_obj_align(brightness_title_label_, LV_ALIGN_CENTER, 0, -100);
        lv_obj_align(brightness_value_label_, LV_ALIGN_CENTER, 0, -80);

        lv_obj_align(feed_interval_title_label_, LV_ALIGN_CENTER, 0, -40);
        lv_obj_align(feed_interval_value_label_, LV_ALIGN_CENTER, 0, -20);

        lv_obj_align(screen_orientation_title_label_, LV_ALIGN_CENTER, 0, 20);
        lv_obj_align(screen_orientation_value_label_, LV_ALIGN_CENTER, 0, 40);

        lv_obj_align(reset_offset_title_label_, LV_ALIGN_CENTER, 0, 80);
        lv_obj_align(reset_offset_value_label_, LV_ALIGN_CENTER, 0, 100);
    }
}

void OptionsScreen::init()
{
    options_scr_ = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(options_scr_, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(options_scr_, LV_OPA_COVER, 0);

    brightness_title_label_ = lv_label_create(options_scr_);
    lv_label_set_long_mode(brightness_title_label_, LV_LABEL_LONG_CLIP);
    lv_obj_set_style_text_color(brightness_title_label_, lv_palette_main(LV_PALETTE_RED), 0);
    lv_label_set_text(brightness_title_label_, "Brightness");

    brightness_value_label_ = lv_label_create(options_scr_);
    lv_label_set_long_mode(brightness_value_label_, LV_LABEL_LONG_CLIP);
    lv_obj_set_style_text_color(brightness_value_label_, lv_color_black(), 0);

    feed_interval_title_label_ = lv_label_create(options_scr_);
    lv_label_set_long_mode(feed_interval_title_label_, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_color(feed_interval_title_label_, lv_color_black(), 0);
    lv_label_set_text(feed_interval_title_label_, "Feed interval");

    feed_interval_value_label_ = lv_label_create(options_scr_);
    lv_label_set_long_mode(feed_interval_value_label_, LV_LABEL_LONG_CLIP);
    lv_obj_set_style_text_color(feed_interval_value_label_, lv_color_black(), 0);

    screen_orientation_title_label_ = lv_label_create(options_scr_);
    lv_label_set_long_mode(screen_orientation_title_label_, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_color(screen_orientation_title_label_, lv_color_black(), 0);
    lv_label_set_text(screen_orientation_title_label_, "Rotate display");

    screen_orientation_value_label_ = lv_label_create(options_scr_);
    lv_label_set_long_mode(screen_orientation_value_label_, LV_LABEL_LONG_CLIP);
    lv_obj_set_style_text_color(screen_orientation_value_label_, lv_color_black(), 0);

    reset_offset_title_label_ = lv_label_create(options_scr_);
    lv_label_set_long_mode(reset_offset_title_label_, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_color(reset_offset_title_label_, lv_color_black(), 0);
    lv_label_set_text(reset_offset_title_label_, "Reset offset");

    reset_offset_value_label_ = lv_label_create(options_scr_);
    lv_label_set_long_mode(reset_offset_value_label_, LV_LABEL_LONG_CLIP);
    lv_obj_set_style_text_color(reset_offset_value_label_, lv_color_black(), 0);

    option_brightness_.label = brightness_title_label_;
    option_brightness_.label_state = brightness_value_label_;
    option_brightness_.action = ScreenAction::CHANGE_BRIGHTNESS;

    option_feed_interval_.label = feed_interval_title_label_;
    option_feed_interval_.label_state = feed_interval_value_label_;
    option_feed_interval_.action = ScreenAction::INCREMENT_FEED_INTERVAL;

    option_screen_orientation_.label = screen_orientation_title_label_;
    option_screen_orientation_.label_state = screen_orientation_value_label_;
    option_screen_orientation_.action = ScreenAction::ROTATE_DISPLAY;

    option_reset_offset_.label = reset_offset_title_label_;
    option_reset_offset_.label_state = reset_offset_value_label_;
    option_reset_offset_.action = ScreenAction::RESET_OFFSET;
}

void OptionsScreen::show()
{
    lv_screen_load(options_scr_);
}

void OptionsScreen::update()
{
    for (uint8_t i = 0; i < kNumOptions; i++) {
        if (i == selected_index_) {
            lv_obj_set_style_text_color(options_[i]->label, lv_palette_main(LV_PALETTE_RED), 0);
        } else {
            lv_obj_set_style_text_color(options_[i]->label, lv_color_black(), 0); 
        }
    }
    lv_label_set_text_fmt(brightness_value_label_, "%d", ui_state_.brightness);
    lv_label_set_text_fmt(feed_interval_value_label_, "%d hrs", ui_state_.feed_interval);
    lv_label_set_text_fmt(reset_offset_value_label_, "+%d hrs", ui_state_.reset_day_offset);
    switch (ui_state_.screen_orientation) {
        case LV_DISPLAY_ROTATION_0: lv_label_set_text(screen_orientation_value_label_, "0"); break;
        case LV_DISPLAY_ROTATION_90: lv_label_set_text(screen_orientation_value_label_, "90"); break;
        case LV_DISPLAY_ROTATION_180: lv_label_set_text(screen_orientation_value_label_, "180"); break;
        case LV_DISPLAY_ROTATION_270: lv_label_set_text(screen_orientation_value_label_, "270"); break;
    }
}

ScreenAction OptionsScreen::primary_action()
{
    ScreenAction action = options_[selected_index_]->action;
    return action;
}

ScreenAction OptionsScreen::secondary_action()
{
    uint8_t index = (++selected_index_) % kNumOptions;
    selected_index_ = index;
    update();
    return ScreenAction::NONE;
}

/*
-------------------------------STATS SCREEN-------------------------------
*/

void StatsScreen::apply_layout()
{
    if (ui_state_.screen_orientation == LV_DISPLAY_ROTATION_90 || ui_state_.screen_orientation == LV_DISPLAY_ROTATION_270) {
        lv_obj_set_style_text_font(stats_label_text_, &lv_font_montserrat_24, 0);
        lv_label_set_long_mode(stats_label_text_, LV_LABEL_LONG_CLIP);
        lv_obj_align(stats_label_text_, LV_ALIGN_LEFT_MID, 20, -35);

        lv_obj_set_style_text_font(stats_label_value_, &lv_font_montserrat_24, 0);
        lv_label_set_long_mode(stats_label_value_, LV_LABEL_LONG_CLIP);
        lv_obj_align(stats_label_value_, LV_ALIGN_LEFT_MID, 20, 0);
    } else {
        lv_obj_set_style_text_font(stats_label_text_, &lv_font_montserrat_24, 0);
        lv_label_set_long_mode(stats_label_text_, LV_LABEL_LONG_WRAP);
        lv_obj_align(stats_label_text_, LV_ALIGN_CENTER, 0, -80);

        lv_obj_set_style_text_font(stats_label_value_, &lv_font_montserrat_24, 0);
        lv_label_set_long_mode(stats_label_value_, LV_LABEL_LONG_WRAP);
        lv_obj_align(stats_label_value_, LV_ALIGN_CENTER, 0, -50);
    }
}

void StatsScreen::init()
{
    stats_scr_ = lv_obj_create(NULL);

    lv_obj_set_style_bg_color(stats_scr_, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(stats_scr_, LV_OPA_COVER, 0);

    stats_label_text_ = lv_label_create(stats_scr_);
    lv_obj_set_style_text_color(stats_label_text_, lv_color_black(), 0);
    lv_label_set_text(stats_label_text_, "Total feedings");

    stats_label_value_ = lv_label_create(stats_scr_);
    lv_obj_set_style_text_color(stats_label_value_, lv_color_black(), 0);
    lv_label_set_text(stats_label_value_, "x");

    apply_layout();
}

void StatsScreen::show()
{
    lv_screen_load(stats_scr_);
}

void StatsScreen::update()
{

}

ScreenAction StatsScreen::primary_action()
{
    return ScreenAction::NONE;
}

ScreenAction StatsScreen::secondary_action()
{
    return ScreenAction::NONE;
}
