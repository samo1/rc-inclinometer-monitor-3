#include <Arduino.h>
#include "pin_config.h"
#include <TFT_eSPI.h>
#include "bluetooth.h"
#include "png_images.h"

#define HEADER_FONT 4

TFT_eSPI tft = TFT_eSPI();
int lcdBacklightBrightness = 120;

Bluetooth bluetooth;

static void drawBatteryIndicator() {
    double batteryVoltage = (analogRead(4) * 2 * 3.3) / 4096;

    double batteryPercentage = (batteryVoltage - 3.7) / (4.2 - 3.7);
    if (batteryPercentage < 0) {
        batteryPercentage = 0;
    }
    if (batteryPercentage > 1) {
        batteryPercentage = 1;
    }

    int batteryIndicatorWidth = static_cast<int>(24 * batteryPercentage);
    tft.fillRect(276 + batteryIndicatorWidth, 16, 24 - batteryIndicatorWidth, 8, TFT_BLACK);
    tft.fillRect(276, 16, batteryIndicatorWidth, 8, TFT_GREEN);
}

void setup() {
    pinMode(PIN_POWER_ON, OUTPUT);
    digitalWrite(PIN_POWER_ON, HIGH);

    analogReadResolution(12);

    tft.init();
    tft.setRotation(1);

    ledcSetup(0, 10000, 8);
    ledcAttachPin(38, 0);
    ledcWrite(0, lcdBacklightBrightness);

    tft.fillScreen(TFT_BLACK);
    tft.fillRect(0, 0, 320, 36, TFT_DARKGREY);

    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
    tft.drawString("Bluetooth", 16, 8, HEADER_FONT);

    tft.fillRect(272, 12, 32, 16, TFT_WHITE);
    tft.fillRect(304, 16, 4, 8, TFT_WHITE);
    drawBatteryIndicator();

    tft.fillRect(0, 36, 320, 170 - 36, TFT_BLACK);
    drawBluetoothSearchingImage(136, 79);
    bluetooth.initialize();
    tft.fillRect(136, 79, 48, 48, TFT_BLACK);
    drawBluetoothDisabledImage(136, 79);
}

void loop() {
    drawBatteryIndicator();
    delay(5000);
}
