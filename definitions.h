#pragma once

constexpr int gmtOffset_sec = 3600;
constexpr int daylightOffset_sec = 3600; // DAYLIGHTSAVINGS = 3600
constexpr const char* ntpServer = "pool.ntp.org";

struct Day {
  tm day;
};

// key = year * 10000 + month * 100 + day;