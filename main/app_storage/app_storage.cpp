#include "app_storage.h"

#include "esp_vfs.h"
#include "esp_vfs_fat.h"

static const char *TAG = "app_storage";

AppStorage::AppStorage() : s_wl_handle(WL_INVALID_HANDLE) {}

void AppStorage::init()
{
    ESP_LOGI(TAG, "Mounting FAT filesystem");
    const esp_vfs_fat_mount_config_t mount_config = {
            .format_if_mount_failed = true,
            .max_files = 4,
            .allocation_unit_size = CONFIG_WL_SECTOR_SIZE,
            .use_one_fat = false,
    };

    esp_err_t err = esp_vfs_fat_spiflash_mount_rw_wl(base_path, "storage", &mount_config, &s_wl_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount FATFS (%s)", esp_err_to_name(err));
    }
}

esp_err_t AppStorage::write_stats(day_data_t* data)
{
    ESP_LOGI(TAG, "Opening file");
    FILE* file = fopen(file_path, "ab");
    if (file == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return ESP_FAIL;
    }
    fwrite(data, sizeof(day_data_t), 1, file);
    fclose(file);
    ESP_LOGI(TAG, "File written");

    return ESP_OK;
}

esp_err_t AppStorage::load_stats(stats_t* stats)
{
    ESP_LOGI(TAG, "Reading file");
    FILE* file = fopen(file_path, "rb");
    if (file == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return ESP_FAIL;
    }
    day_data_t data;
    while (fread(&data, sizeof(day_data_t), 1, file) == 1) {
        stats->tot_num_feeds += data.num_feeds;
    }
    fclose(file);
    ESP_LOGI(TAG, "File read");

    return ESP_OK;
}
