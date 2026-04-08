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
    lv_obj_set_style_text_font(main_label_text, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(main_label_text, lv_color_black(), 0);
    lv_obj_align(main_label_text, LV_ALIGN_LEFT_MID, 20, -15);
    lv_label_set_text(main_label_text, "Todays number of feedings");

    main_label_count = lv_label_create(main_scr);
    lv_obj_set_style_text_font(main_label_count, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(main_label_text, lv_color_black(), 0);
    lv_obj_align(main_label_count, LV_ALIGN_CENTER, 50, -15);
    lv_label_set_text_fmt(main_label_count, "%d", counter);

    main_label_bar = lv_bar_create(main_scr);
    lv_obj_set_style_bg_color(main_label_bar, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(main_label_bar, lv_color_black(), 0);
    lv_obj_align(main_label_bar, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_set_size(main_label_bar, 150, 20);
    lv_bar_set_range(main_label_bar, 0, 10);
    lv_bar_set_value(main_label_bar, counter, LV_ANIM_ON);

    counter = 0;
}

void MainScreen::show()
{
    lv_screen_load(main_scr);
}

void MainScreen::update()
{
    uint8_t count = 0;
    lv_label_set_text_fmt(main_label_count, "%d", counter);
    lv_bar_set_value(main_label_bar, counter, LV_ANIM_ON);
}

ScreenAction MainScreen::on_short_press()
{
    return ScreenAction::INCREMENT_FEEDS;
}

void MainScreen::on_long_press()
{

}

void MainScreen::update_count(uint8_t count)
{
    counter = count;
    update();
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
    lv_obj_set_style_text_font(options_label_1_text, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(options_label_1_text, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_align(options_label_1_text, LV_ALIGN_LEFT_MID, 30, -35);
    lv_label_set_text(options_label_1_text, "Landscape:");

    options_variable_1_text = lv_label_create(options_scr);
    lv_label_set_long_mode(options_variable_1_text, LV_LABEL_LONG_CLIP);
    lv_obj_set_style_text_font(options_variable_1_text, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(options_variable_1_text, lv_color_black(), 0);
    lv_obj_align(options_variable_1_text, LV_ALIGN_LEFT_MID, 210, -35);

    options_label_2_text = lv_label_create(options_scr);
    lv_label_set_long_mode(options_label_2_text, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_font(options_label_2_text, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(options_label_2_text, lv_color_black(), 0);
    lv_obj_align(options_label_2_text, LV_ALIGN_LEFT_MID, 30, 0);
    lv_label_set_text(options_label_2_text, "Feed interval:");

    options_variable_2_text = lv_label_create(options_scr);
    lv_label_set_long_mode(options_variable_2_text, LV_LABEL_LONG_CLIP);
    lv_obj_set_style_text_font(options_variable_2_text, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(options_variable_2_text, lv_color_black(), 0);
    lv_obj_align(options_variable_2_text, LV_ALIGN_LEFT_MID, 210, 0);

    options_label_3_text = lv_label_create(options_scr);
    lv_label_set_long_mode(options_label_3_text, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_font(options_label_3_text, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(options_label_3_text, lv_color_black(), 0);
    lv_obj_align(options_label_3_text, LV_ALIGN_LEFT_MID, 30, 35);
    lv_label_set_text(options_label_3_text, "Animal:");

    options_variable_3_text = lv_label_create(options_scr);
    lv_label_set_long_mode(options_variable_3_text, LV_LABEL_LONG_CLIP);
    lv_obj_set_style_text_font(options_variable_3_text, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(options_variable_3_text, lv_color_black(), 0);
    lv_obj_align(options_variable_3_text, LV_ALIGN_LEFT_MID, 210, 35);

    option_1_.label = options_label_1_text;
    option_1_.label_state = options_variable_1_text;
    option_1_.action = ScreenAction::OPTION_1_ACTION;

    option_2_.label = options_label_2_text;
    option_2_.label_state = options_variable_2_text;
    option_2_.action = ScreenAction::OPTION_2_ACTION;

    option_3_.label = options_label_3_text;
    option_3_.label_state = options_variable_3_text;
    option_3_.action = ScreenAction::OPTION_3_ACTION;
}

void OptionsScreen::show()
{
    lv_screen_load(options_scr);
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
    lv_label_set_text_fmt(options_variable_1_text, "%s", (settings_.landscape_orientation ? "TRUE" : "FALSE"));
    lv_label_set_text_fmt(options_variable_2_text, "%d hrs", settings_.feed_interval);
    switch (settings_.animal) {
        case Animals::CAT: {
            lv_label_set_text(options_variable_3_text, "cat");
            break;
        }
        case Animals::DOG: {
            lv_label_set_text(options_variable_3_text, "dog");
            break;
        }
        case Animals::HUMAN: {
            lv_label_set_text(options_variable_3_text, "human");
            break;
        }
    }
}

ScreenAction OptionsScreen::on_short_press()
{
    ScreenAction action = options_[selected_index_]->action;
    return action;
}

void OptionsScreen::on_long_press()
{
    uint8_t index = (++selected_index_) % NUM_OPTIONS;
    selected_index_ = index;
    update();
}

void OptionsScreen::update_settings(settings_t settings)
{
    settings_ = settings;
    update();
}
