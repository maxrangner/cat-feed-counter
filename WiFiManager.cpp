#include <Arduino.h>
#include "WiFi.h"
#include "WiFiManager.h"
#include "credentials.h"

WiFiManager::WiFiManager() : status(WL_DISCONNECTED) {
}

void WiFiManager::setup() {
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(WIFI_SSID);
  status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nYou're connected to the network");
}