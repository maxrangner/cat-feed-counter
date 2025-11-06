#include "time.h"
#include "SystemManager.h"
#include "Button.h"
#include "Display.h"
#include "definitions.h"



void setup() {
  Serial.begin(9600);
  while(!Serial) delay(10);
  Serial.print("Boot!\n");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  static SystemManager SysMgr;
  static Display Display(SysMgr);
  static Button BuiltInButton(9, 50, true);

  BuiltInButton.update();
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

  Display.drawScreen();
}