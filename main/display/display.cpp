#include "display.h"

#include <cstring>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_commands.h"
#include "esp_lcd_panel_vendor.h"

static esp_lcd_panel_io_handle_t io_handle = NULL;
static esp_lcd_panel_handle_t panel_handle = NULL;

#define WAVESHARE_LEDC_DUTY_RESOLUTION LEDC_TIMER_13_BIT
#define WAVESHARE_LEDC_MAX_DUTY ((1U << WAVESHARE_LEDC_DUTY_RESOLUTION) - 1U)

static void display_apply_waveshare_panel_init()
{
    ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(io_handle, LCD_CMD_SLPOUT, NULL, 0));
    vTaskDelay(pdMS_TO_TICKS(100));

    ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(io_handle, 0x36, (uint8_t[]) {0x00}, 1));
    ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(io_handle, 0x3A, (uint8_t[]) {0x55}, 1));
    ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(io_handle, 0xB0, (uint8_t[]) {0x00, 0xE8}, 2));
    ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(io_handle, 0xB2, (uint8_t[]) {0x0C, 0x0C, 0x00, 0x33, 0x33}, 5));
    ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(io_handle, 0xB7, (uint8_t[]) {0x75}, 1));
    ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(io_handle, 0xBB, (uint8_t[]) {0x1A}, 1));
    ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(io_handle, 0xC0, (uint8_t[]) {0x80}, 1));
    ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(io_handle, 0xC2, (uint8_t[]) {0x01, 0xFF}, 2));
    ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(io_handle, 0xC3, (uint8_t[]) {0x13}, 1));
    ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(io_handle, 0xC4, (uint8_t[]) {0x20}, 1));
    ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(io_handle, 0xC6, (uint8_t[]) {0x0F}, 1));
    ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(io_handle, 0xD0, (uint8_t[]) {0xA4, 0xA1}, 1));
    ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(io_handle, 0xE0, (uint8_t[]) {0xD0, 0x0D, 0x14, 0x0D, 0x0D, 0x09, 0x38, 0x44, 0x4E, 0x3A, 0x17, 0x18, 0x2F, 0x30}, 14));
    ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(io_handle, 0xE1, (uint8_t[]) {0xD0, 0x09, 0x0F, 0x08, 0x07, 0x14, 0x37, 0x44, 0x4D, 0x38, 0x15, 0x16, 0x2C, 0x2E}, 14));
    ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(io_handle, LCD_CMD_INVON, NULL, 0));
    ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(io_handle, LCD_CMD_DISPON, NULL, 0));
    ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(io_handle, LCD_CMD_RAMWR, NULL, 0));
}

static void display_set_waveshare_panel_mode()
{
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, false));
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, true, false));
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));
    ESP_ERROR_CHECK(esp_lcd_panel_set_gap(panel_handle, 34, 0));
}

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
    io_config.pclk_hz           = 12 * 1000 * 1000;
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
    panel_config.color_space    = ESP_LCD_COLOR_SPACE_BGR;
    panel_config.bits_per_pixel = 16;

    ESP_ERROR_CHECK(
        esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle)
    );

    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    display_apply_waveshare_panel_init();
    display_set_waveshare_panel_mode();

    // With panel inversion enabled, 0xFFFF displays as black on screen.
    static uint16_t line[LCD_H_RES];
    memset(line, 0xFF, sizeof(line));
    for (int y = 0; y < LCD_V_RES; y++) {
        ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(panel_handle, 0, y, LCD_H_RES, y + 1, line));
    }

    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
}

static void display_backlight_init()
{
    gpio_config_t bk_gpio_config = {};
    bk_gpio_config.mode = GPIO_MODE_OUTPUT;
    bk_gpio_config.pin_bit_mask = 1ULL << PIN_NUM_BL;
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));

    // ESP_ERROR_CHECK(gpio_set_level((gpio_num_t)PIN_NUM_BL, 1));

    ledc_timer_config_t timer_config = {};
    timer_config.speed_mode = LEDC_LOW_SPEED_MODE;
    timer_config.timer_num = LEDC_TIMER_0;
    timer_config.duty_resolution = WAVESHARE_LEDC_DUTY_RESOLUTION;
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
    ESP_ERROR_CHECK(ledc_fade_func_install(0));
}

static void display_lvgl_init()
{
    lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    lvgl_cfg.task_stack = 8192;
    lvgl_cfg.task_max_sleep_ms = 10;
    ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));

    lvgl_port_display_cfg_t disp_cfg = {};

    disp_cfg.io_handle     = io_handle;
    disp_cfg.panel_handle  = panel_handle;
    disp_cfg.buffer_size   = LCD_BUF_SIZE;
    disp_cfg.double_buffer = false;
    disp_cfg.hres          = LCD_H_RES;
    disp_cfg.vres          = LCD_V_RES;
    disp_cfg.color_format  = LV_COLOR_FORMAT_RGB565;
    // Native portrait diagnostic baseline to isolate rotation/gap issues.
    disp_cfg.rotation.mirror_x = true;
    disp_cfg.rotation.mirror_y = false;
    disp_cfg.rotation.swap_xy = false;
    disp_cfg.flags.buff_dma = true;
    disp_cfg.flags.swap_bytes = false;
    disp_cfg.flags.sw_rotate = true;

    lv_display_t *disp_handle = lvgl_port_add_disp(&disp_cfg);
    ESP_ERROR_CHECK(disp_handle != NULL ? ESP_OK : ESP_FAIL);

    lvgl_port_lock(portMAX_DELAY);
    lv_display_set_rotation(disp_handle, LV_DISPLAY_ROTATION_90);
    lvgl_port_unlock();
}

void display_set_brightness(uint8_t percent)
{
    if (percent > 100) {
        percent = 100;
    }

    uint32_t duty = WAVESHARE_LEDC_MAX_DUTY - (81U * (100U - percent));
    if (percent == 0) {
        duty = 0;
    }

    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
}

void display_init()
{
    display_spi_init();
    display_io_init();
    display_panel_init();
    display_backlight_init();
    display_lvgl_init();
}
