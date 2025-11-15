#pragma once
#include <vector>
#include "time.h"
#include "WiFiManager.h"

class Display;

class SystemManager {
  WiFiManager wifi;
  int counter;
  std::vector<time_t> feedTimes;
public:
  SystemManager();
  void setup(Display& disp);
  int getCount() const;
  const bool feedTimesIsEmpty() const;
  const time_t& getLastFeedTime() const;
  time_t getTime();
  void increment();
  void reset();
  void limiter();
};
