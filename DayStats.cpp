#include "DayStats.h"
#include "utils.h"

const int DayStats::getDay() const {
  return this->day;
}

const int DayStats::getNumFeeds() const {
  int numFeeds = 0;
  for (auto& f : this->feeds) {
    numFeeds++;
  }
  return numFeeds;
}

void DayStats::setDay(time_t timestamp) {
  day = getConvertedDay(timestamp);
}

void DayStats::addFeeds(std::vector<time_t>& dayFeeds) {
  feeds = dayFeeds;
}

void DayStats::printStats() {
  int count = 0;
  for (auto& t : feeds) {
    Serial.print(count++);
    Serial.print(": ")
    Serial.println(t);
  }
  Serial.println("*-----------*");
}