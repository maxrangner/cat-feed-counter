#include <Arduino.h>
#include "Button.h"

Button::Button(uint8_t pin, int setDebounce, bool hasPullup) : pin_(pin), debounce(setDebounce) {
  if (hasPullup) {
    pinMode(pin, INPUT_PULLUP);
    buttonState = digitalRead(pin_);
    prevButtonState = buttonState;
  } else {
    pinMode(pin, INPUT);
    buttonState = digitalRead(pin_);
    prevButtonState = buttonState;
  }
  prevTime = 0;
  buttonHeldTimer = 0;
  now = millis();
  holdThreashold = 300;
  pushedFlag = false;
  heldFlag = false;
}

bool Button::isDebounced() {
  if (buttonState != prevButtonState) {
    if (now - prevTime >= debounce) {
      prevTime = now;
      return true;
    }
  }
  return false;
}

void Button::update() {
  pushedFlag = false;
  heldFlag = false;
  now = millis();
  buttonState = digitalRead(pin_);
  if (isDebounced()) {
    if (buttonState == 0 && prevButtonState == 1) buttonHeldTimer = now;
    if (buttonState == 1 && prevButtonState == 0) {
      if (now - buttonHeldTimer >= holdThreashold) heldFlag = true;
      else pushedFlag = true;
    }
  }
  prevButtonState = buttonState;
}

bool Button::wasPressed() {
  return pushedFlag;
}

bool Button::wasHeld() {
  return heldFlag;
}
