#pragma once

class Button {
  // BUTTON
  uint8_t pin_;
  bool buttonState;
  bool prevButtonState;
  // DEBOUNCE
  unsigned long prevTime;
  unsigned long debounce;
  unsigned long buttonHeldTimer;
  unsigned long now;
  unsigned long holdThreashold;
  bool pushedFlag;
  bool heldFlag;
public:
  // CONSTRUCTORS
  Button(uint8_t pin, int setDebounce, bool hasPullup = false);
  // CORE
  bool isDebounced();
  void update();
  bool wasPressed();
  bool wasHeld();
};

