#include <Arduino.h>
#include "Counter.h"

int Counter::getCount() const {
  return counter;
}

void Counter::increment() {
  Serial.print("increment\n");
  counter++;
  limiter();
}

void Counter::reset() {
  counter = 0;
}

void Counter::limiter() {
  if (counter >= 10) counter = 0;
}