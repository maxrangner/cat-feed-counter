#include "display.h"

#include <cstring>

#include "driver/spi_master.h"
#include "driver/ledc.h"

#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"

#include "config.h"

static esp_lcd_panel_io_handle_t io_handle = NULL;
static esp_lcd_panel_handle_t panel_handle = NULL;
static lv_display_t* disp_handle = NULL;

static void display_spi_init()
{
    spi_bus_config_t buscfg = {};

    buscfg.mosi_io_num    = PIN_NUM_MOSI;
    buscfg.miso_io_num    = -1;
    buscfg.sclk_io_num    = PIN_NUM_CLK;
    buscfg.quadwp_io_num  = -1;
    buscfg.quadhd_io_num  = -1;
    buscfg.max_transfer_sz = LCD_BUF_SIZE * 2;

    ESP_ERROR_CHECK(
        spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO)
    );
}

static void display_io_init()
{
    esp_lcd_panel_io_spi_config_t io_config = {};

    io_config.dc_gpio_num       = PIN_NUM_DC;
    io_config.cs_gpio_num       = PIN_NUM_CS;
    io_config.pclk_hz           = DISPLAY_SPI_PIXEL_CLOCK_HZ;
    io_config.spi_mode          = 0;
    io_config.trans_queue_depth = 10;
    io_config.lcd_cmd_bits      = 8;
    io_config.lcd_param_bits    = 8;

    ESP_ERROR_CHECK(
        esp_lcd_new_panel_io_spi(SPI2_HOST, &io_config, &io_handle)
    );
}

static void display_panel_init(void)
{
    esp_lcd_panel_dev_config_t panel_config = {};

    panel_config.reset_gpio_num = PIN_NUM_RST;
    panel_config.color_space    = ESP_LCD_COLOR_SPACE_RGB;
    panel_config.bits_per_pixel = 16;

    ESP_ERROR_CHECK(
        esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle)
    );

    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));

    ESP_ERROR_CHECK(esp_lcd_panel_set_gap(panel_handle, 34, 0));
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));

    static uint16_t line[LCD_H_RES];
    memset(line, 0xFF, sizeof(line));
    for (int y = 0; y < LCD_V_RES; y++) {
        ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(panel_handle, 0, y, LCD_H_RES, y + 1, line));
    }

    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
}

static void display_backlight_init()
{
    ledc_timer_config_t timer_config = {};
    timer_config.speed_mode = LEDC_LOW_SPEED_MODE;
    timer_config.timer_num = LEDC_TIMER_0;
    timer_config.duty_resolution = LEDC_TIMER_10_BIT;
    timer_config.freq_hz = 5000;
    timer_config.clk_cfg = LEDC_AUTO_CLK;

    ESP_ERROR_CHECK(ledc_timer_config(&timer_config));

    ledc_channel_config_t channel_config = {};
    channel_config.gpio_num = PIN_NUM_BL;
    channel_config.speed_mode = LEDC_LOW_SPEED_MODE;
    channel_config.channel = LEDC_CHANNEL_0;
    channel_config.intr_type = LEDC_INTR_DISABLE;
    channel_config.timer_sel = LEDC_TIMER_0;
    channel_config.duty = 0;
    channel_config.hpoint = 0;

    ESP_ERROR_CHECK(ledc_channel_config(&channel_config));
}

static void display_lvgl_init()
{
    lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    lvgl_cfg.task_stack = 8192;
    // lvgl_cfg.task_max_sleep_ms = 10;
    ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));

    lvgl_port_display_cfg_t disp_cfg = {};

    disp_cfg.io_handle     = io_handle;
    disp_cfg.panel_handle  = panel_handle;
    disp_cfg.buffer_size   = LCD_BUF_SIZE;
    disp_cfg.double_buffer = false;
    disp_cfg.hres          = LCD_H_RES;
    disp_cfg.vres          = LCD_V_RES;
    disp_cfg.color_format  = LV_COLOR_FORMAT_RGB565;
    disp_cfg.rotation.mirror_x = false;
    disp_cfg.rotation.mirror_y = false;
    disp_cfg.rotation.swap_xy = false;
    disp_cfg.flags.buff_dma = true;
    disp_cfg.flags.swap_bytes = true;
    disp_cfg.flags.sw_rotate = true;

    disp_handle = lvgl_port_add_disp(&disp_cfg);
    ESP_ERROR_CHECK(disp_handle != NULL ? ESP_OK : ESP_FAIL);
    lv_display_set_rotation(disp_handle, LV_DISPLAY_ROTATION_90);
}

void display_set_brightness(uint8_t percent)
{
    if (percent > 100) percent = 100;

    uint32_t duty = (BACKLIGHT_MAX_DUTY * percent) / 100U;
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
}

void display_set_rotation(lv_display_rotation_t rotation)
{
    lv_display_set_rotation(disp_handle, rotation);
}

void display_init()
{
    display_spi_init();
    display_io_init();
    display_panel_init();
    display_backlight_init();
    display_lvgl_init();
}
