#include "time.h"
#include "SystemManager.h"
#include "Button.h"
#include "Display.h"
#include "definitions.h"

SystemManager SysMgr;
Display Display(SysMgr);
Button BuiltInButton(9, 50, true);

void setup() {
  Serial.begin(9600);
  while(!Serial) delay(10);
  Serial.print("Boot!\n");
  SysMgr.setup(Display);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  updateButtons();
  
  if (BuiltInButton.wasPressed()) {
    switch (Display.getCurrentScreen()) {
      case MenuScreen::mainScreen: SysMgr.increment(); break;
      case MenuScreen::statsScreen: break;
      case MenuScreen::settingsScreen: Display.switchAnimal(); break;
    }
  }
  if (BuiltInButton.wasHeld()) {
    Display.switchScreen();
  }

  SysMgr.checkDay();
  Display.drawScreen();
}

void updateButtons() {
  BuiltInButton.update();
}