#include "info_screen.h"
#include <cstdio>

#define BLE_CMD_RESET_SPEED "reset_speed"
#define BLE_CMD_SET_WHEEL_SIZE "set_wheel_size "

double wheelSize1 = 49.500;
double wheelSize2 = 43.478;

bool InfoScreen::Callback() {
    if (stateManager.getState() == State::info) {
        uint16_t batVoltRaw = batteryTask.getRawValue();
        uint16_t batteryMinutes = batteryTask.getBatteryTimeMinutes();
        double distance = Bluetooth::getDistance();
        unsigned long tickNr = Bluetooth::getTickNr();
        mmDistancePerRevolution = Bluetooth::getmmDistancePerRevolution();
        bool stateChangedNow = stateManager.stateChangedNow();
        if (batVoltRaw != previousBatVoltRaw || batteryMinutes != previousBatteryMinutes
            || distance != previousDistance || tickNr != previousTickNr
            || mmDistancePerRevolution != previousMmDistancePerRevolution
            || selection != previousSelection
            || stateChangedNow) {

            if (!displayInitialized || stateChangedNow) {
                DisplayMainArea::drawInfoScreenInitial();
                displayInitialized = true;
            }
            if (batVoltRaw != previousBatVoltRaw || stateChangedNow) {
                DisplayMainArea::drawInfoScreenBatSensor(batVoltRaw);
            }
            if (batteryMinutes != previousBatteryMinutes || stateChangedNow) {
                DisplayMainArea::drawInfoScreenBatTime(batteryMinutes);
            }
            if (distance != previousDistance || stateChangedNow) {
                DisplayMainArea::drawInfoScreenDistance(distance);
            }
            if (tickNr != previousTickNr || stateChangedNow) {
                DisplayMainArea::drawInfoScreenTickNr(tickNr);
            }
            if (mmDistancePerRevolution != previousMmDistancePerRevolution || stateChangedNow) {
                DisplayMainArea::drawInfoScreenDpr(mmDistancePerRevolution);
            }
            if (selection != previousSelection || stateChangedNow) {
                DisplayMainArea::drawInfoScreenSelection(selection);
            }
            previousBatVoltRaw = batVoltRaw;
            previousBatteryMinutes = batteryMinutes;
            previousDistance = distance;
            previousTickNr = tickNr;
            previousMmDistancePerRevolution = mmDistancePerRevolution;
            previousSelection = selection;
        }
    } else {
        displayInitialized = false;
    }
    return true;
}

void InfoScreen::handleButtonClick() {
    switch (selection) {
        case InfoScreenSelection::NONE:
            selection = InfoScreenSelection::BATTERY;
            break;
        case InfoScreenSelection::BATTERY:
            selection = InfoScreenSelection::WHEEL_SIZE;
            break;
        case InfoScreenSelection::WHEEL_SIZE:
            selection = InfoScreenSelection::DISTANCE;
            break;
        case InfoScreenSelection::DISTANCE:
            selection = InfoScreenSelection::NONE;
            break;
    }
}

void InfoScreen::handleButtonLongPress() {
    switch (selection) {
        case InfoScreenSelection::NONE:
            break;
        case InfoScreenSelection::BATTERY:
            batteryTask.resetBatteryTime();
            break;
        case InfoScreenSelection::WHEEL_SIZE:
            if (mmDistancePerRevolution == wheelSize1) {
                mmDistancePerRevolution = wheelSize2;
            } else {
                mmDistancePerRevolution = wheelSize1;
            }
            setWheelSize(mmDistancePerRevolution);
            break;
        case InfoScreenSelection::DISTANCE:
            resetDistance();
            break;
    }
    selection = InfoScreenSelection::NONE;
}

void InfoScreen::resetDistance() {
    bluetooth.sendCommand(BLE_CMD_RESET_SPEED);
}

void InfoScreen::setWheelSize(double wheelSize) {
    char cmd[32];
    snprintf(cmd, sizeof(cmd), "set_wheel_size %.1f", wheelSize);
    bluetooth.sendCommand(cmd);
}
