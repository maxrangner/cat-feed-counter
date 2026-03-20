idf_component_register(
    SRCS "main.cpp" "display/display.cpp" "app_controller/app_controller.cpp" "screens/screens.cpp"
    INCLUDE_DIRS "." "display" "app_controller" "screens"
    REQUIRES esp_driver_spi esp_driver_gpio esp_lcd lvgl esp_lvgl_port
)