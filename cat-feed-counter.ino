#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "Counter.h"
#include "Debouncer.h"
#include "Roboto_Medium20pt7b.h"

// SCREEN
#define TFT_MOSI  6    // LCD MOSI
#define TFT_SCLK  7    // LCD SCLK
#define TFT_CS    14   // LCD_CS
#define TFT_DC    15   // LCD_DC
#define TFT_RST   21   // LCD_RST
#define TFT_BL    22   // LCD_BL (backlight)

// CONSTS
#define SCREEN_ROTATION 1
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 172

Adafruit_ST7789 tft(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(9600);
  while(!Serial) delay(10);
  SPI.begin(TFT_SCLK, -1, TFT_MOSI);
  // Tänd bakgrundsbelysningen
  pinMode(TFT_BL, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(TFT_BL, HIGH);

  tft.init(SCREEN_HEIGHT, SCREEN_WIDTH);
  tft.setRotation(SCREEN_ROTATION);

  tft.fillScreen(ST77XX_WHITE);
  // tft.setFont(&Roboto_Medium20pt7b);
  // tft.setTextColor(ST77XX_WHITE);
  // tft.setCursor(10, 60);
  // tft.println("ESP32-C6");
  // tft.setCursor(10, 110);
  // tft.println("ST7789 172x320");
  // Serial.print("Hello world!");
}

void loop() {
  static uint16_t x = 10, y = 80, w = 40, h = 20;
  static Counter Counter;
  static Debouncer debouncerPIN9(50, 1);
  static bool invert = false;
  tft.fillRect(x, y, w, h, invert ? ST77XX_RED : ST77XX_BLUE);

  if (debouncerPIN9.check() {
    invert = !invert;
  }
}

/*
TODO:
- Button pressed debounce logic. Merge with debounce class?
*/
