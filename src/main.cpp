#include <Arduino.h>
#include <OneButton.h>
#include "pin_config.h"
#include <TFT_eSPI.h>
#include "battery_task.h"
#include "bluetooth.h"
#include "debug.h"
#include "dig.h"
#include "display_header.h"
#include "display_main_area.h"
#include "inclinometer.h"
#include "power_saving_task.h"
#include "state.h"
#include "task_scheduler.h"
#include "winch.h"

TFT_eSPI tft = TFT_eSPI();

StateManager stateManager;
DisplayMainArea displayMainArea;
BatteryTask batteryTask;
PowerSavingTask powerSavingTask;
Bluetooth bluetooth(stateManager);
Inclinometer inclinometer(stateManager, displayMainArea);
Winch winch(stateManager, displayMainArea, bluetooth);
Dig dig(stateManager, displayMainArea, bluetooth);
OneButton buttonUp(BUTTON_2);
OneButton buttonDown(BUTTON_1);

static void handleButtonUpClick() {
    bool lcdOff = powerSavingTask.isLcdOff();
    powerSavingTask.wakeUp();
    if (!lcdOff) {
        stateManager.goToNextState();
        DisplayMainArea::clear();
    }
}

static void handleButtonDownClick() {
    bool lcdOff = powerSavingTask.isLcdOff();
    powerSavingTask.wakeUp();
    if (!lcdOff) {
        winch.handleButtonClick();
        dig.handleButtonClick();
    }
}

void setup() {
    DEBUG_INIT;
    pinMode(LCD_POWER_ON, OUTPUT);
    digitalWrite(LCD_POWER_ON, HIGH);

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
    winch.enable();
    dig.enable();
    buttonUp.attachClick(handleButtonUpClick);
    buttonDown.attachClick(handleButtonDownClick);
    powerSavingTask.wakeUp();
}

void loop() {
    scheduler.execute();
    bluetooth.loop();
    buttonUp.tick();
    buttonDown.tick();

    bool wakeup = inclinometer.isWarning()
                  || ((stateManager.getState() == State::winch) && Bluetooth::getWinchEnabled())
                  || ((stateManager.getState() == State::dig) && Bluetooth::getFrontDigEnabled())
                  || batteryTask.getBatteryPercentage() == 0.0;
    if (wakeup) {
        powerSavingTask.wakeUp();
    }
}
