#pragma once

class Debouncer {
  unsigned long  prevTime;
  unsigned long  debounce;
  bool buttonState;
public:
  Debouncer(int setDebounce, bool restingState = 0);
  bool check();
  void setButtonState();
};

