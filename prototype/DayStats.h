#pragma once
#include "time.h"
#include "definitions.h"

class DayStats {
  time_t feeds[MAX_FEEDS];
public:
  const int getNumFeeds() const;
  void addFeeds(time_t (&dayFeeds)[MAX_FEEDS]);
  void printStats();
};