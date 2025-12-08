#include <Arduino.h>
#include "time.h"
#include "SystemManager.h"
#include "Display.h"
#include "utils.h"

SystemManager::SystemManager() : counter(0) {
}

void SystemManager::setup(Display& disp) {
  wifi.setup();

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
  if (feedTimes[0] == 0) return true;
  return false;
}

const time_t& SystemManager::getLastFeedTime() const {
  // if (counter == 0) return 0;
  // return feedTimes[counter - 1]; // Dangerous out of bounds risk
  return feedTimes[0];
}

// CORE
void SystemManager::increment() {
  feedTimes[counter] = getTime();
  Serial.print("counter: "); Serial.print(counter); Serial.print(": ");
  counter++;
  this->limiter();
  this->debugPrintFeedTimes();
}

void SystemManager::resetDay() {
  Serial.println("ResetDay()");
  this->updatePreviousFeedTimes();
  this->debugPrintPreviousFeedTimes();
  this->resetFeedTimes();
  counter = 0;
}

void SystemManager::resetFeedTimes() {
  for (auto& t : feedTimes) {
    t = 0;
  }
}

void SystemManager::updatePreviousFeedTimes() {
  for (int day = 0; day < (DAYS_SAVED - 1); day++) { // Move every day one step back
    for (int time = 0; time < MAX_FEEDS; time++) {
      previousFeedTimes[day][time] = previousFeedTimes[day + 1][time];
    }
  }
  for (int time = 0; time < MAX_FEEDS; time++) { // Place newest day last
    previousFeedTimes[DAYS_SAVED - 1][time] = feedTimes[time];
  }
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
  //   this->resetDay();
  // }
}

void SystemManager::debugPrintFeedTimes() {
  for (auto& t : feedTimes) {
    Serial.print(t); Serial.print("   ");
  }
  Serial.println(" ");
}

void SystemManager::debugPrintPreviousFeedTimes() {
  int day = 0;
  for (auto& d : previousFeedTimes) {
    Serial.print(day++); Serial.print(": ");
    for (auto& t : d) {
      Serial.print(t); Serial.print("  ");
    }
    Serial.println(" ");
  }
}
