#include <Arduino.h>
#include <ezButton.h>
#include "pin_config.h"
#include <TFT_eSPI.h>
#include "battery_task.h"
#include "bluetooth.h"
#include "debug.h"
#include "display_header.h"
#include "display_main_area.h"
#include "inclinometer.h"
#include "power_saving_task.h"
#include "task_scheduler.h"

TFT_eSPI tft = TFT_eSPI();

DisplayMainArea displayMainArea;
BatteryTask batteryTask;
PowerSavingTask powerSavingTask;
Bluetooth bluetooth;
Inclinometer inclinometer(bluetooth, displayMainArea);
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

    displayMainArea.init();

    DisplayHeader::init();
    DisplayHeader::printText("Starting...");
    DisplayHeader::drawBluetoothImage();
    DisplayHeader::drawBatteryOutline();

    batteryTask.measureAndDrawBatteryIndicator();
    delay(500);
    batteryTask.measureAndDrawBatteryIndicator();
    batteryTask.enable();

    bluetooth.initialize();
    inclinometer.enable();
    powerSavingTask.wakeUp();
}

void loop() {
    scheduler.execute();
    buttonUp.loop();
    buttonDown.loop();
    bluetooth.loop();

    auto wakeup = buttonUp.isPressed() || buttonDown.isPressed() || inclinometer.isWarning()
                  || batteryTask.getBatteryPercentage() == 0.0;

    if (wakeup) {
        powerSavingTask.wakeUp();
    }
}
