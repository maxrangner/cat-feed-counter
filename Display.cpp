#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <string>
#include "Display.h"
#include "fonts/Heavitas70pt7b.h"
#include "fonts/Comfortaa_Regular12pt7b.h"
#include "fonts/Comfortaa_Regular20pt7b.h"
#include "fonts/Comfortaa_Regular70pt7b.h"

// SCREEN
#define TFT_MOSI  6    // LCD MOSI
#define TFT_SCLK  7    // LCD SCLK
#define TFT_CS    14   // LCD_CS
#define TFT_DC    15   // LCD_DC
#define TFT_RST   21   // LCD_RST
#define TFT_BL    22   // LCD_BL (backlight)

// CONSTS
const uint16_t SCREEN_ROTATION = 1;
const uint16_t SCREEN_WIDTH = 320;
const uint16_t SCREEN_HEIGHT = 172;

Display::Display(Counter& Counter) : tft(TFT_CS, TFT_DC, TFT_RST), connectedCounter(&Counter) {
  // SCREEN SETUP
  SPI.begin(TFT_SCLK, -1, TFT_MOSI);
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);  // Tänd bakgrundsbelysningen
  tft.init(SCREEN_HEIGHT, SCREEN_WIDTH);
  tft.setRotation(SCREEN_ROTATION);
  tft.fillScreen(ST77XX_WHITE);
  // VARIABLES
  tempCounter = 0;
  currentMenu = MenuScreen::mainScreen;
}

void Display::switchScreen() {
  currentMenu = static_cast<MenuScreen>((static_cast<int>(currentMenu) + 1) % 3);
}

void Display::mainScreen() {
  GFXcanvas16 canvas(SCREEN_WIDTH, SCREEN_HEIGHT);

  canvas.fillScreen(ST77XX_WHITE);
  canvas.setTextColor(ST77XX_BLACK);
  canvas.setFont(&Comfortaa_Regular12pt7b);
  canvas.setCursor(20, 30);
  canvas.print("Previous:");
  canvas.setFont(&Comfortaa_Regular20pt7b);
  canvas.setCursor(20, 80);
  canvas.print("12:56");
  canvas.setFont(&Heavitas70pt7b);
  canvas.setCursor(200, 135);
  canvas.print(connectedCounter->getCount());
  tft.drawRGBBitmap(0, 0, canvas.getBuffer(), canvas.width(), canvas.height());
}

void Display::statsScreen() {
  GFXcanvas16 canvas(SCREEN_WIDTH, SCREEN_HEIGHT);

  canvas.fillScreen(ST77XX_WHITE);
  canvas.setTextColor(ST77XX_BLACK);
  canvas.setFont(&Comfortaa_Regular12pt7b);
  canvas.setCursor(20, 30);
  canvas.print("Stats:");
  canvas.setFont(&Comfortaa_Regular20pt7b);
  canvas.setCursor(20, 80);
  canvas.print("1001 meals total");
  tft.drawRGBBitmap(0, 0, canvas.getBuffer(), canvas.width(), canvas.height());
}

void Display::settingsScreen() {
  GFXcanvas16 canvas(SCREEN_WIDTH, SCREEN_HEIGHT);

  canvas.fillScreen(ST77XX_WHITE);
  canvas.setTextColor(ST77XX_BLACK);
  canvas.setFont(&Comfortaa_Regular12pt7b);
  canvas.setCursor(20, 30);
  canvas.print("Settings:");
  canvas.setFont(&Comfortaa_Regular20pt7b);
  canvas.setCursor(20, 80);
  canvas.print("Animal: cat");
  tft.drawRGBBitmap(0, 0, canvas.getBuffer(), canvas.width(), canvas.height());
}

void Display::drawScreen() {
  switch (currentMenu) {
    case MenuScreen::mainScreen: mainScreen(); break;
    case MenuScreen::statsScreen: statsScreen(); break;
    case MenuScreen::settingsScreen: settingsScreen(); break;
  }
}

