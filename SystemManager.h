#pragma once
#include "time.h"
#include "src/WiFiManager.h"
#include "DayStats.h"

class Display;

class SystemManager {
  WiFiManager wifi;
  uint8_t counter;
  time_t feedTimesArray[9] = {0};
public:
  time_t previousDay;

  SystemManager();
  void setup(Display& disp);
  
  // GETTERS
  int getCount() const;
  const bool feedTimesIsEmpty() const;
  const time_t& getLastFeedTime() const;

  // CORE
  void increment();
  void resetDay();

  // UTILS
  void limiter();
  void checkDay();
  void printAllStats();
};
