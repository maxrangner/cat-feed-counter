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
  return feedTimes[counter - 1]; // Dangerous out of bounds risk
}

// CORE
void SystemManager::increment() {
  feedTimes[counter++] = getTime();
  limiter();
  Serial.print("counter: "); Serial.print(counter);
  Serial.println(" Increment counter.");
  printAllStats();
}

void SystemManager::resetDay() {
  Serial.println("ResetDay()");
  // counter = 0;
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
  for (auto& t : feedTimes) {
    Serial.print(t); Serial.print("   ");
  }
  Serial.println(" ");
}
