#pragma once
#include <Adafruit_ST7789.h>
#include "Counter.h"

enum class MenuScreen {
  mainScreen,
  statsScreen,
  settingsScreen
};

class Display {
  Adafruit_ST7789 tft;
  Counter* connectedCounter;
  uint16_t tempCounter;
  MenuScreen currentMenu;
  // MENU
  void mainScreen();
  void statsScreen();
  void settingsScreen();
public:
  Display(Counter& Counter);
  // GETTERS / SETTERS
  void switchScreen();
  const MenuScreen& getScreen() const;
  void switchAnimal();

  // DRAW
  void drawScreen();
};