#include "info_screen.h"

bool InfoScreen::Callback() {
    if (stateManager.getState() == State::info) {
        uint16_t batVoltRaw = batteryTask.getRawValue();
        uint16_t batteryMinutes = batteryTask.getBatteryTimeMinutes();
        unsigned long tickNr = Bluetooth::getTickNr();
        bool stateChangedNow = stateManager.stateChangedNow();
        if (batVoltRaw != previousBatVoltRaw || batteryMinutes != previousBatteryMinutes || tickNr != previousTickNr
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
            if (tickNr != previousTickNr || stateChangedNow) {
                DisplayMainArea::drawInfoScreenTickNr(tickNr);
            }
            previousBatVoltRaw = batVoltRaw;
            previousBatteryMinutes = batteryMinutes;
            previousTickNr = tickNr;
        }
    } else {
        displayInitialized = false;
    }
    return true;
}
