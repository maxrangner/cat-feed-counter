#include <Arduino.h>
#include "Debouncer.h"
#define BUTTON_PIN 9

Debouncer::Debouncer(int setDebounce, bool restingState) : debounce(setDebounce), buttonState(restingState) {
  prevTime = 0;
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

bool Debouncer::check() {
  long unsigned now = millis();
  if (now - prevTime >= debounce) {
    prevTime = now;
    return true;
  }
  return false;
}

void Debouncer::setButtonState() {

}