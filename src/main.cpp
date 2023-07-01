#include <Arduino.h>
#include "pin_config.h"
#include "TFT_eSPI.h"

#define DISPLAY_HEIGHT 170
#define DISPLAY_WIDTH 320
#define DISPLAY_BRIGHTNESS_MAX 252
#define DISPLAY_BRIGHTNESS_MIN 0

TFT_eSPI tft = TFT_eSPI();
int lcdBacklightBrightness = 128;

void setup() {
    pinMode(PIN_POWER_ON, OUTPUT);
    digitalWrite(PIN_POWER_ON, HIGH);

    analogReadResolution(12);

    tft.init();
    tft.setRotation(1);

    ledcSetup(0, 10000, 8);
    ledcAttachPin(38, 0);
    ledcWrite(0, lcdBacklightBrightness);

    tft.setTextSize(2);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("Ahoj", 16, 16, 2);
}

void loop() {
    double batteryVoltage = (analogRead(4) * 2 * 3.3) / 4096;

    tft.drawString(String(batteryVoltage) + " V", 16, 64, 1);

    delay(5000);
}
