#ifndef SCREENS_H
#define SCREENS_H

#include <cstdint>

#include "lvgl.h"

// typedef struct {
//     display_option
//     action_callback
// } options_t;

class Screen {
public:
    virtual void init() = 0;
    virtual void enter() = 0;
    virtual void render() = 0;
    virtual void on_short_press() = 0;
    virtual void on_long_press() = 0;
};

class MainScreen : public Screen {
    lv_obj_t* main_scr;
    lv_obj_t* main_label_text;
    lv_obj_t* main_label_count;
    lv_obj_t* main_label_bar;
public:
    void init() override;
    void enter() override;
    void render() override;
    void on_short_press() override;
    void on_long_press() override;
};

class OptionsScreen : public Screen {
    lv_obj_t* options_scr;
    lv_obj_t* options_label_1_text;
    lv_obj_t* options_variable_1_text;
    lv_obj_t* options_label_2_text;

    uint8_t selected_index_;
public:
    void init() override;
    void enter() override;
    void render() override;
    void on_short_press() override;
    void on_long_press() override;
};

#endif