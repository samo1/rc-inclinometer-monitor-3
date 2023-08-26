#include <Arduino.h>
#include <TaskSchedulerDeclarations.h>
#include <ezButton.h>
#include "pin_config.h"
#include <TFT_eSPI.h>
#include "battery_task.h"
#include "bluetooth.h"
#include "debug.h"
#include "display_header.h"
#include "display_main_area.h"
#include "power_saving_task.h"

TFT_eSPI tft = TFT_eSPI();

Scheduler scheduler;
BatteryTask batteryTask(&scheduler);
PowerSavingTask powerSavingTask(&scheduler);
Bluetooth bluetooth;
ezButton buttonUp(BUTTON_2);
ezButton buttonDown(BUTTON_1);

void setup() {
    DEBUG_INIT;
    pinMode(LCD_POWER_ON, OUTPUT);
    digitalWrite(LCD_POWER_ON, HIGH);

    buttonUp.setDebounceTime(50);
    buttonDown.setDebounceTime(50);

    analogReadResolution(12);

    tft.init();
    tft.setRotation(1);

    DisplayMainArea::init();

    DisplayHeader::init();
    DisplayHeader::printText("Starting...");
    DisplayHeader::drawBluetoothImage();
    DisplayHeader::drawBatteryOutline();

    BatteryTask::measureAndDrawBatteryIndicator();
    delay(500);
    BatteryTask::measureAndDrawBatteryIndicator();
    batteryTask.enable();

    bluetooth.initialize();

    powerSavingTask.wakeUp();
}

void loop() {
    scheduler.execute();
    buttonUp.loop();
    buttonDown.loop();
    bluetooth.loop();

    if (buttonUp.isPressed() || buttonDown.isPressed()) {
        powerSavingTask.wakeUp();
    }
}
