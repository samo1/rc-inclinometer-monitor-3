#define _TASK_OO_CALLBACKS

#include <Arduino.h>
#include <TaskScheduler.h>
#include "pin_config.h"
#include <TFT_eSPI.h>
#include "battery_task.h"
#include "bluetooth.h"
#include "png_images.h"

#define DISPLAY_BRIGHTNESS_MAX 252
#define HEADER_FONT 4

TFT_eSPI tft = TFT_eSPI();
int lcdBacklightBrightness = DISPLAY_BRIGHTNESS_MAX;

Scheduler scheduler;
BatteryTask batteryTask(&scheduler, &tft);
Bluetooth bluetooth;

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
    batteryTask.drawBatteryIndicator();

    tft.fillRect(0, 36, 320, 170 - 36, TFT_BLACK);

    delay(500);
    batteryTask.drawBatteryIndicator();
    batteryTask.enable();

    drawBluetoothSearchingImage(136, 79);
    bluetooth.initialize();
    tft.fillRect(136, 79, 48, 48, TFT_BLACK);
    drawBluetoothDisabledImage(136, 79);
}

void loop() {
    scheduler.execute();
}
