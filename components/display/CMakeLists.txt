idf_component_register(
    SRCS "display.cpp"
    INCLUDE_DIRS "."
    REQUIRES esp_driver_spi esp_driver_gpio esp_lcd lvgl esp_lvgl_port
) 