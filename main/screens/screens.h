#ifndef SCREENS_H
#define SCREENS_H

#define NUM_OPTIONS 3

#include <cstdint>

#include "lvgl.h"
#include "app_types.h"

enum class ScreenAction {
    INCREMENT_FEEDS,
    OPTION_1_ACTION,
    OPTION_2_ACTION,
    OPTION_3_ACTION,
};

typedef struct {
    lv_obj_t* label;
    lv_obj_t* label_state;
    ScreenAction action;
} options_t;

class Screen {
public:
    virtual void init() = 0;
    virtual void show() = 0;
    virtual void update() = 0;
    virtual ScreenAction on_short_press() = 0;
    virtual void on_long_press() = 0;
};

class MainScreen : public Screen {
    lv_obj_t* main_scr;
    lv_obj_t* main_label_text;
    lv_obj_t* main_label_count;
    lv_obj_t* main_label_bar;

    uint8_t counter = 0;
public:
    void init() override;
    void show() override;
    void update() override;
    ScreenAction on_short_press() override;
    void on_long_press() override;
    void update_count(uint8_t count);
};

class OptionsScreen : public Screen {
    lv_obj_t* options_scr;
    lv_obj_t* options_label_1_text;
    lv_obj_t* options_variable_1_text;
    lv_obj_t* options_label_2_text;
    lv_obj_t* options_variable_2_text;
    lv_obj_t* options_label_3_text;
    lv_obj_t* options_variable_3_text;

    settings_t settings_;

    options_t option_1_;
    options_t option_2_;
    options_t option_3_;
    uint8_t selected_index_ = 0;
    options_t* options_[NUM_OPTIONS] = {&option_1_, &option_2_, &option_3_};
public:
    void init() override;
    void show() override;
    void update() override;
    ScreenAction on_short_press() override;
    void on_long_press() override;
    void update_settings(settings_t settings);
};

#endif
