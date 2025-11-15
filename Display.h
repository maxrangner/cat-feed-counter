#pragma once
#include <Adafruit_ST7789.h>

class SystemManager;

enum class MenuScreen {
  mainScreen,
  statsScreen,
  settingsScreen
};

class Display {
  Adafruit_ST7789 tft;
  SystemManager* connectedMgr;
  uint16_t tempCounter;
  unsigned long prevScreenUpdate;
  MenuScreen currentMenu;
  // MENU
  void mainScreen();
  void statsScreen();
  void settingsScreen();
public:
  Display(SystemManager& SysMgr);
  // GETTERS / SETTERS
  void switchScreen();
  const MenuScreen& getCurrentScreen() const;
  void switchAnimal();
  void displayMessage(uint16_t x = 0, uint16_t y = 0, String message = "");

  // DRAW
  void drawScreen();
};