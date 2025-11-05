#include "Counter.h"

int Counter::getCounter() const {
  return counter;
}

void Counter::addCount() {
  counter++;
}