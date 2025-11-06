#include <Arduino.h>
#include "time.h"
#include "SystemManager.h"

SystemManager::SystemManager() {
  wifi.setup();
  counter = 0;
}

// GETTERS
int SystemManager::getCount() const {
  return counter;
}

const bool SystemManager::feedTimesIsEmpty() const {
  return (feedTimes.empty()) ? true : false;
}

const time_t& SystemManager::getLastFeedTime() const {
  return feedTimes.back();
}

time_t SystemManager::getTime() {
  return time(nullptr);
}

// CORE
void SystemManager::increment() {
  Serial.println("increment");
  counter++;
  limiter();
  feedTimes.emplace_back(time(nullptr));
}

void SystemManager::reset() {
  counter = 0;
}

// UTILS
void SystemManager::limiter() {
  if (counter >= 10) counter = 0;
}