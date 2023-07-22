#include <Arduino.h>
#include <TaskSchedulerDeclarations.h>
#include <ezButton.h>
#include "pin_config.h"
#include <TFT_eSPI.h>
#include "battery_task.h"
#include "bluetooth.h"
#include "power_saving_task.h"
#include "png_images.h"

#define HEADER_FONT 4

TFT_eSPI tft = TFT_eSPI();

Scheduler scheduler;
BatteryTask batteryTask(&scheduler);
PowerSavingTask powerSavingTask(&scheduler);
Bluetooth bluetooth;
ezButton buttonUp(BUTTON_2);
ezButton buttonDown(BUTTON_1);

static void printHeaderText(const char* str) {
    tft.fillRect(0, 0, 272, 36, TFT_DARKGREY);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
    tft.drawString(str, 16, 8, HEADER_FONT);
}

static void clearMainArea() {
    tft.fillRect(0, 36, 320, 170 - 36, TFT_BLACK);
}

void setup() {
    pinMode(LCD_POWER_ON, OUTPUT);
    digitalWrite(LCD_POWER_ON, HIGH);

    buttonUp.setDebounceTime(50);
    buttonDown.setDebounceTime(50);

    analogReadResolution(12);

    tft.init();
    tft.setRotation(1);

    tft.fillScreen(TFT_BLACK);
    tft.fillRect(0, 0, 320, 36, TFT_DARKGREY);

    printHeaderText("Bluetooth");

    tft.fillRect(272, 12, 32, 16, TFT_WHITE);
    tft.fillRect(304, 16, 4, 8, TFT_WHITE);
    batteryTask.drawBatteryIndicator();

    clearMainArea();

    delay(500);
    batteryTask.drawBatteryIndicator();
    batteryTask.enable();

    drawBluetoothSearchingImage(136, 79);
    bluetooth.initialize();
    tft.fillRect(136, 79, 48, 48, TFT_BLACK);
    drawBluetoothDisabledImage(136, 79);

    powerSavingTask.wakeUp();
}

void loop() {
    scheduler.execute();
    buttonUp.loop();
    buttonDown.loop();

    if (buttonUp.isPressed() || buttonDown.isPressed()) {
        powerSavingTask.wakeUp();
    }
}
