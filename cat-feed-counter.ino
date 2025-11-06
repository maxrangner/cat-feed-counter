#include "Counter.h"
#include "Button.h"
#include "Display.h"

void setup() {
  Serial.begin(9600);
  while(!Serial) delay(10);
  Serial.print("Boot!\n");
}

void loop() {
  static Counter Counter;
  static Display Display(Counter);
  static Button BuiltInButton(9, 50, true);

  BuiltInButton.update();

  if (BuiltInButton.wasPressed()) {
    switch (Display.getScreen()) {
      case MenuScreen::mainScreen: Counter.increment(); break;
      case MenuScreen::statsScreen: break;
      case MenuScreen::settingsScreen: Display.switchAnimal(); break;
    }
  }
  if (BuiltInButton.wasHeld()) {
    Display.switchScreen();
  }
  Display.drawScreen();
}