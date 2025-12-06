#include <Arduino.h>
#include "time.h"
#include "SystemManager.h"
#include "Display.h"
#include "utils.h"

SystemManager::SystemManager() {
}

void SystemManager::setup(Display& disp) {
  wifi.setup();
  counter = 0;
  previousDay = getTime();

  while (time(nullptr) <= 1000) { // If time not synced properly it returns 0, but checking for <= 1000 gives some margin.
    disp.displayMessage(15, 90, "Syncing time...");
    delay(1000);
  }
}

// GETTERS
int SystemManager::getCount() const {
  return counter;
}

const bool SystemManager::feedTimesIsEmpty() const {
  if (feedTimesArray[0] == 0) return true;
  return false;
}

const time_t& SystemManager::getLastFeedTime() const {
  return feedTimesArray[counter-1];
}

// CORE
void SystemManager::increment() {
  Serial.println("Increment counter.");
  feedTimesArray[counter++] = time(nullptr);
  limiter();
}

void SystemManager::resetDay() {
  Serial.println("ResetDay()");
  // counter = 0;
  // DayStats newStats;
  
  // if (!feedTimesCurrentDay.empty()) {
  //   time_t newDay = feedTimesCurrentDay[0];
  //   newStats.setDay(newDay);
  //   newStats.addFeeds(feedTimesCurrentDay);
  //   statistics.push_back(newStats);
  //   previousDay = getTime();
  //   printAllStats();
  // }
  // feedTimesCurrentDay.clear();
}

// UTILS
void SystemManager::limiter() {
  if (counter >= 10) counter = 0;
}

void SystemManager::checkDay() {
  time_t now = getTime();
  // Serial.print("now: ");
  // Serial.println(getConvertedDay(now));
  // Serial.print("previousDay: ");
  // Serial.println(getConvertedDay(previousDay));

  // if (getConvertedDay(now) != getConvertedDay(previousDay)) {
  //   resetDay();
  // }
}

void SystemManager::printAllStats() {
  // for (auto& s : statistics) {
  //   s.printStats();
  // }
}
