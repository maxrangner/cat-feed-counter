#ifndef SCREENS_H
#define SCREENS_H

#define NUM_OPTIONS 4

#include <cstdint>
#include <ctime>

#include "lvgl.h"
#include "app_types.h"

enum class ScreenAction {
    NONE,
    INCREMENT_FEEDS,
    SAVE_DATA,
    CHANGE_BRIGHTNESS,
    INCREMENT_FEED_INTERVAL,
    ROTATE_DISPLAY,
    RESET_OFFSET,
};

typedef struct {
    uint8_t count;
    uint8_t feed_interval;
    bool half_feed_steps;
    uint8_t brightness;
    lv_display_rotation_t screen_orientation;
    uint8_t reset_day_offset;
    time_t last_feed_time;
} ui_state_t;

typedef struct {
    lv_obj_t* label;
    lv_obj_t* label_state;
    ScreenAction action;
} options_t;

class Screen {
protected:
    ui_state_t ui_state_ = {};
    virtual void apply_layout() = 0;
public:
    virtual void init() = 0;
    virtual void show() = 0;
    virtual void update() = 0;
    virtual ScreenAction primary_action() = 0;
    virtual ScreenAction secondary_action() = 0;
    void update_ui_state(const app_state_t& state);
};

class MainScreen : public Screen {
    lv_obj_t* main_scr_;
    lv_obj_t* main_label_text_;
    lv_obj_t* main_label_time_;
    lv_obj_t* main_label_count_;
    lv_obj_t* main_label_bar_;
    void apply_layout() override;
public:
    void init() override;
    void show() override;
    void update() override;
    ScreenAction primary_action() override;
    ScreenAction secondary_action() override;
};

class OptionsScreen : public Screen {
    lv_obj_t* options_scr_;

    lv_obj_t* brightness_title_label_;
    lv_obj_t* brightness_value_label_;

    lv_obj_t* feed_interval_title_label_;
    lv_obj_t* feed_interval_value_label_;

    lv_obj_t* screen_orientation_title_label_;
    lv_obj_t* screen_orientation_value_label_;

    lv_obj_t* reset_offset_title_label_;
    lv_obj_t* reset_offset_value_label_;

    options_t option_brightness_;
    options_t option_feed_interval_;
    options_t option_screen_orientation_;
    options_t option_reset_offset_;
    uint8_t selected_index_ = 0;
    
    options_t* options_[NUM_OPTIONS] = {
        &option_brightness_,
        &option_feed_interval_,
        &option_screen_orientation_,
        &option_reset_offset_,
    };
    void apply_layout() override;
public:
    void init() override;
    void show() override;
    void update() override;
    ScreenAction primary_action() override;
    ScreenAction secondary_action() override;
};

class StatsScreen : public Screen {
    lv_obj_t* stats_scr_;
    lv_obj_t* stats_label_text_;
    lv_obj_t* stats_label_value_;
    void apply_layout() override;
public:
    void init() override;
    void show() override;
    void update() override;
    ScreenAction primary_action() override;
    ScreenAction secondary_action() override;
};

#endif
