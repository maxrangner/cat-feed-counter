#pragma once
#include <Adafruit_ST7789.h>
#include "SystemManager.h"

enum class MenuScreen {
  mainScreen,
  statsScreen,
  settingsScreen
};

class Display {
  Adafruit_ST7789 tft;
  SystemManager* connectedMgr;
  uint16_t tempCounter;
  MenuScreen currentMenu;
  // MENU
  void mainScreen();
  void statsScreen();
  void settingsScreen();
public:
  Display(SystemManager& SysMgr);
  // GETTERS / SETTERS
  void switchScreen();
  const MenuScreen& getScreen() const;
  void switchAnimal();

  // DRAW
  void drawScreen();
};