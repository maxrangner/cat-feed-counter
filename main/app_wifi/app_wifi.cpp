#include "app_wifi.h"
#include "esp_log.h"
#include "credentials.h"
#include "app_types.h"

static const char *TAG = "app_wifi interface";

AppWifi::AppWifi() {}

void AppWifi::init(QueueHandle_t app_queue)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    wifi_config_t credentials = {
        .sta = {
            .ssid = SSID,
            .password = PASSWORD,
        }
    };

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &credentials));
    ESP_ERROR_CHECK(esp_wifi_start());
    esp_wifi_set_ps(WIFI_PS_MIN_MODEM);

    app_queue_ = app_queue;
    esp_event_handler_register(
        WIFI_EVENT,
        ESP_EVENT_ANY_ID,
        &wifi_event_cb,
        this
    );

    esp_event_handler_register(
        IP_EVENT,
        IP_EVENT_STA_GOT_IP,
        &wifi_event_cb,
        this
    );

    esp_wifi_connect();

    ESP_LOGI(TAG, "app_wifi.init() finished.");
}

void AppWifi::wifi_event_cb(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    auto* self = static_cast<AppWifi*>(arg);

    app_event packet;
    packet.msg_event = AppEventType::WIFI_UPDATE;

    if (event_base == WIFI_EVENT) {
        if (event_id == WIFI_EVENT_STA_START)
            packet.wifi_state = WifiState::DISCONNECTED;
        if (event_id == WIFI_EVENT_STA_DISCONNECTED)
            packet.wifi_state = WifiState::DISCONNECTED;
    }
    else if (event_base == IP_EVENT) {
        if (event_id == IP_EVENT_STA_GOT_IP)
            packet.wifi_state = WifiState::CONNECTED_STA;
    }

    xQueueSend(self->app_queue_, &packet, 0);
}

void AppWifi::connect()
{
    esp_wifi_connect();
}