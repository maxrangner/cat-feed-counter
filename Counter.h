#pragma once

class Counter {
  int counter;
public:
  int getCount() const;
  void increment();
  void reset();
  void limiter();
};
