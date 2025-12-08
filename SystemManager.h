#pragma once
#include "time.h"
#include "src/WiFiManager.h"
#include "DayStats.h" 
#include "definitions.h"

class Display;

class SystemManager {
  WiFiManager wifi;
  uint8_t counter;
  time_t feedTimes[MAX_FEEDS] = {0};
  time_t previousFeedTimes[DAYS_SAVED][MAX_FEEDS]; // 30 days
public:
  SystemManager();
  void setup(Display& disp);
  
  // GETTERS
  int getCount() const;
  const bool feedTimesIsEmpty() const;
  const time_t& getLastFeedTime() const;

  // CORE
  void increment();
  void resetDay();
  void resetFeedTimes();
  void updatePreviousFeedTimes();

  // UTILS
  void limiter();
  void checkDay();
  void printFeedTimes();
  void printPreviousFeedTimes();
};
