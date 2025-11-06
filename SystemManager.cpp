#include <Arduino.h>
#include "time.h"
#include "SystemManager.h"

int SystemManager::getCount() const {
  return counter;
}

const bool SystemManager::feedTimesIsEmpty() const {
  return (feedTimes.empty()) ? true : false;
}

const time_t& SystemManager::getLastFeedTime() const {
  return feedTimes.back();
}

void SystemManager::increment() {
  Serial.println("increment");
  counter++;
  limiter();
  feedTimes.emplace_back(time(nullptr));
}

void SystemManager::reset() {
  counter = 0;
}

void SystemManager::limiter() {
  if (counter >= 10) counter = 0;
}