#include "display.h"

#include "driver/spi_master.h"
#include "driver/gpio.h"

#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"

static esp_lcd_panel_io_handle_t io_handle = NULL;
static esp_lcd_panel_handle_t panel_handle = NULL;

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
    io_config.pclk_hz           = 40 * 1000 * 1000;
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

    esp_lcd_panel_set_gap(panel_handle, 0, 34);
    esp_lcd_panel_invert_color(panel_handle, true);

    // Clear GRAM before backlight turns on
    uint16_t *line = (uint16_t *)heap_caps_calloc(240, sizeof(uint16_t), MALLOC_CAP_DMA);
    if (line) {
        memset(line, 0xFF, 240 * sizeof(uint16_t));  // 0xFFFF = white in RGB565
        for (int y = 0; y < LCD_V_RES; y++) {
            esp_lcd_panel_draw_bitmap(panel_handle, 0, y, 240, y + 1, line);
        }
        free(line);
    }

    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

    gpio_set_direction((gpio_num_t)PIN_NUM_BL, GPIO_MODE_OUTPUT);
    gpio_set_level((gpio_num_t)PIN_NUM_BL, 1);
}

static void display_lvgl_init()
{
    lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    // TODO: Find higher value that still gives smooth screen updates. 500?
    lvgl_cfg.task_max_sleep_ms = 500; 
    ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));

    lvgl_port_display_cfg_t disp_cfg = {};

    disp_cfg.io_handle     = io_handle;
    disp_cfg.panel_handle  = panel_handle;
    disp_cfg.buffer_size   = LCD_BUF_SIZE;
    disp_cfg.double_buffer = true;
    disp_cfg.hres          = LCD_H_RES;
    disp_cfg.vres          = LCD_V_RES;
    disp_cfg.rotation.mirror_x = false;
    disp_cfg.rotation.mirror_y = true;
    disp_cfg.rotation.swap_xy = true;
    disp_cfg.flags.buff_dma = true;

    lv_disp_t *disp_handle = lvgl_port_add_disp(&disp_cfg);
}

void display_init()
{
    display_spi_init();
    display_io_init();
    display_panel_init();
    display_lvgl_init();
}
