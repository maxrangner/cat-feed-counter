idf_component_register(
    SRCS "main.cpp" "display/display.cpp" "app_controller/app_controller.cpp" "screens/screens.cpp" "app_wifi/app_wifi.cpp"
    INCLUDE_DIRS "." "display" "app_controller" "screens" "app_wifi"
    REQUIRES esp_driver_spi esp_driver_gpio esp_lcd lvgl esp_lvgl_port esp_wifi esp_event nvs_flash
)