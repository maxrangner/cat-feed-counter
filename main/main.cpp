#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "display.h"

extern "C" void app_main(void)
{
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_handle_t panel_handle = NULL;
    display_init(io_handle, panel_handle);

    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}