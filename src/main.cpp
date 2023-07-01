#include <Arduino.h>
#include "pin_config.h"
#include "TFT_eSPI.h"

TFT_eSPI tft = TFT_eSPI();

void setup() {
    pinMode(PIN_POWER_ON, OUTPUT);
    digitalWrite(PIN_POWER_ON, HIGH);

    tft.init();

    tft.setRotation(1);
    tft.setTextSize(2);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("Ahoj", 16, 16, 2);
}

void loop() {
    delay(1000);
}
