#include "app_storage.h"

#include "nvs.h"
#include "nvs_flash.h"

#include "esp_vfs.h"
#include "esp_vfs_fat.h"

static const char *TAG = "app_storage";
static constexpr char kSettingsNamespace[] = "settings";
static constexpr char kBrightnessKey[] = "brightness";
static constexpr char kHalfFeedStepsKey[] = "half_steps";
static constexpr char kFeedIntervalKey[] = "feed_int";
static constexpr char kDisplayRotationKey[] = "rotation";
static constexpr char kDayResetOffsetKey[] = "reset_off";

static void load_u8_setting(nvs_handle_t handle, const char* key, uint8_t* value)
{
    uint8_t loaded_value = 0;
    esp_err_t err = nvs_get_u8(handle, key, &loaded_value);
    if (err == ESP_OK) {
        *value = loaded_value;
    } else if (err != ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGW(TAG, "Failed to load setting %s (%s)", key, esp_err_to_name(err));
    }
}

AppStorage::AppStorage() : s_wl_handle(WL_INVALID_HANDLE) {}

void AppStorage::init()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "Mounting FAT filesystem");
    esp_vfs_fat_mount_config_t mount_config = {};
    mount_config.format_if_mount_failed = true;
    mount_config.max_files = 4;
    mount_config.allocation_unit_size = CONFIG_WL_SECTOR_SIZE;
    mount_config.use_one_fat = false;

    ESP_LOGI(TAG, "Free heap: %lu", esp_get_free_heap_size());
    esp_err_t err = esp_vfs_fat_spiflash_mount_rw_wl(base_path, "storage", &mount_config, &s_wl_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount FATFS (%s)", esp_err_to_name(err));
    }
}

void AppStorage::load_settings(settings_t* settings)
{
    nvs_handle_t handle = 0;
    esp_err_t err = nvs_open(kSettingsNamespace, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        ESP_LOGI(TAG, "No saved settings found (%s)", esp_err_to_name(err));
        return;
    }

    load_u8_setting(handle, kBrightnessKey, &settings->brightness);

    uint8_t half_feed_steps = settings->half_feed_steps ? 1 : 0;
    load_u8_setting(handle, kHalfFeedStepsKey, &half_feed_steps);
    settings->half_feed_steps = half_feed_steps != 0;

    load_u8_setting(handle, kFeedIntervalKey, &settings->feed_interval);

    uint8_t display_rotation = static_cast<uint8_t>(settings->display_rotation);
    load_u8_setting(handle, kDisplayRotationKey, &display_rotation);
    settings->display_rotation = static_cast<lv_display_rotation_t>(display_rotation);

    load_u8_setting(handle, kDayResetOffsetKey, &settings->day_reset_offset);

    nvs_close(handle);
}

void AppStorage::save_settings(const settings_t* settings)
{
    nvs_handle_t handle = 0;
    esp_err_t err = nvs_open(kSettingsNamespace, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open settings NVS (%s)", esp_err_to_name(err));
        return;
    }

    err = nvs_set_u8(handle, kBrightnessKey, settings->brightness);
    if (err == ESP_OK) err = nvs_set_u8(handle, kHalfFeedStepsKey, settings->half_feed_steps ? 1 : 0);
    if (err == ESP_OK) err = nvs_set_u8(handle, kFeedIntervalKey, settings->feed_interval);
    if (err == ESP_OK) err = nvs_set_u8(handle, kDisplayRotationKey, static_cast<uint8_t>(settings->display_rotation));
    if (err == ESP_OK) err = nvs_set_u8(handle, kDayResetOffsetKey, settings->day_reset_offset);
    if (err == ESP_OK) err = nvs_commit(handle);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to save settings (%s)", esp_err_to_name(err));
    }

    nvs_close(handle);
}

esp_err_t AppStorage::write_stats(const day_data_t* data)
{
    ESP_LOGI(TAG, "Opening file");

    FILE* file = fopen(file_path, "ab");
    if (file == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return ESP_FAIL;
    }

    if (fwrite(data, sizeof(day_data_t), 1, file) != 1) {
        ESP_LOGE(TAG, "Failed to write stats");
        fclose(file);
        return ESP_FAIL;
    }

    fclose(file);
    ESP_LOGI(TAG, "File written: STATS -------------> %lu", data->num_feeds);

    return ESP_OK;
}

esp_err_t AppStorage::load_stats(stats_t* stats)
{
    ESP_LOGI(TAG, "Reading file");
    stats->tot_num_feeds = 0;

    FILE* file = fopen(file_path, "rb");
    if (file == NULL) {
        ESP_LOGI(TAG, "No stats file found, starting fresh");
        return ESP_OK;
    }

    day_data_t data;
    while (fread(&data, sizeof(day_data_t), 1, file) == 1) {
        stats->tot_num_feeds += data.num_feeds;
    }
    fclose(file);
    ESP_LOGI(TAG, "File read");

    return ESP_OK;
}
