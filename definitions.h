#pragma once

constexpr int gmtOffset_sec = 3600;
constexpr int daylightOffset_sec = 3600; // DAYLIGHTSAVINGS = 3600
constexpr const char* ntpServer = "pool.ntp.org";
constexpr const int MAX_FEEDS = 10;
constexpr const int DAYS_SAVED = 30;

struct Day {
  tm day;
};

// key = year * 10000 + month * 100 + day;