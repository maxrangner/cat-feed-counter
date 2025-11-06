#pragma once
#include <vector>
#include "time.h"
#include "WiFiManager.h"

class SystemManager {
  WiFiManager wifi;
  int counter;
  std::vector<time_t> feedTimes;
public:
  SystemManager();
  int getCount() const;
  const bool feedTimesIsEmpty() const;
  const time_t& getLastFeedTime() const;
  time_t getTime();
  void increment();
  void reset();
  void limiter();
};
