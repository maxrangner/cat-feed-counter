#pragma once
#include <vector>
#include "time.h"

class SystemManager {
  int counter;
  std::vector<time_t> feedTimes;
public:
  int getCount() const;
  const bool feedTimesIsEmpty() const;
  const time_t& getLastFeedTime() const;
  void increment();
  void reset();
  void limiter();
};
