#ifndef APP_STORAGE_H
#define APP_STORAGE_H

#include "wear_levelling.h"

#include "app_types.h"

class AppStorage {
    wl_handle_t s_wl_handle;
    const char* base_path = "/storage";
    const char* file_path = "/storage/storage.dat";
public:
    AppStorage();
    void init();
    void load_settings(settings_t* settings);
    void save_settings(const settings_t* settings);
    esp_err_t write_stats(const day_data_t* data);
    esp_err_t load_stats(stats_t* stats);
};

#endif
