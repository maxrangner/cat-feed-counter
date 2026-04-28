#include "app_wifi.h"

#include <cstdlib>
#include <ctime>

#include "esp_log.h"
#include "credentials.h"
#include "app_types.h"
#include "esp_netif_sntp.h"
#include "esp_sntp.h"

#include "config.h"

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

    ESP_LOGI(TAG, "app_wifi.init() finished.");
}

void AppWifi::wifi_event_cb(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    auto* self = static_cast<AppWifi*>(arg);

    if (event_base == WIFI_EVENT) {
        if (event_id == WIFI_EVENT_STA_DISCONNECTED) {
            if (self->reconnect_attempts_ < kMaxReconnectAttempts) {
                self->reconnect_attempts_++;
                ESP_LOGI(TAG, "WiFi disconnected, reconnect attempt %d/%d", self->reconnect_attempts_, kMaxReconnectAttempts);
                esp_wifi_connect();
            } else {
                ESP_LOGW(TAG, "WiFi reconnect failed after %d attempts, giving up", kMaxReconnectAttempts);
            }
        }
    }
    else if (event_base == IP_EVENT) {
        if (event_id == IP_EVENT_STA_GOT_IP) {
            self->reconnect_attempts_ = 0;
            xTaskCreate(
                snpt_task,
                "snptTask",
                4096,
                arg,
                1,
                NULL
            );
        }
    }
}

void AppWifi::snpt_task(void* pvParameters)
{
    auto* self = static_cast<AppWifi*>(pvParameters);

    app_event_t package = {};
    package.msg_event = AppEventType::TIME_SYNCED;
    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");
    esp_netif_sntp_init(&config);

    while(esp_netif_sntp_sync_wait(pdMS_TO_TICKS(10000)) != ESP_OK) {
        printf("Failed to update system time within 10s timeout");
    }
    
    setenv("TZ", kAppTimezone, 1);
    tzset();

    xQueueSend(self->app_queue_, &package, 0);
    esp_wifi_stop();
    esp_netif_sntp_deinit();

    ESP_LOGI(TAG, "SNPT time sync complete");
    vTaskDelete(NULL);
}

void AppWifi::sync_time()
{
    ESP_ERROR_CHECK(esp_wifi_start());
    esp_wifi_connect();
}
