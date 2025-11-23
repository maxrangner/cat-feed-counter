#pragma once
#include <vector>
#include "time.h"
#include "WiFiManager.h"
#include "DayStats.h"

class Display;

class SystemManager {
  WiFiManager wifi;
  int counter;
  std::vector<time_t> feedTimes;
  std::vector<time_t> feedTimesCurrentDay;
  std::vector<DayStats> statistics;
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
