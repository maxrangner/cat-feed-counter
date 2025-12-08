#include "DayStats.h"
#include "utils.h"

const int DayStats::getNumFeeds() const {
  int numFeeds = 0;
  for (auto& f : this->feeds) {
    numFeeds++;
  }
  return numFeeds;
}

void DayStats::addFeeds(time_t (&dayFeeds)[MAX_FEEDS]) {
  for (int i = 0; i < MAX_FEEDS; i++) {
    this->feeds[i] = dayFeeds[i];
  }
}

void DayStats::printStats() {
  int count = 0;
  for (auto& t : feeds) {
    Serial.print(++count);
    Serial.print(": ");
    Serial.println(t);
  }
  Serial.println("*-----------*");
}