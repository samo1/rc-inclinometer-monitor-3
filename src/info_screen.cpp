#include "info_screen.h"

bool InfoScreen::Callback() {
    if (stateManager.getState() == State::info) {
        uint16_t batVoltRaw = batteryTask.getRawValue();
        unsigned long milliseconds = millis();
        unsigned long seconds = milliseconds / 1000;
        unsigned long tickNr = Bluetooth::getTickNr();
        bool stateChangedNow = stateManager.stateChangedNow();
        if (batVoltRaw != previousBatVoltRaw || seconds != previousSeconds || tickNr != previousTickNr
            || stateChangedNow) {
            if (!displayInitialized || stateChangedNow) {
                DisplayMainArea::drawInfoScreenInitial();
                displayInitialized = true;
            }
            if (batVoltRaw != previousBatVoltRaw || stateChangedNow) {
                DisplayMainArea::drawInfoScreenBatSensor(batVoltRaw);
            }
            if (seconds != previousSeconds || stateChangedNow) {
                DisplayMainArea::drawInfoScreenTime(seconds);
            }
            if (tickNr != previousTickNr || stateChangedNow) {
                DisplayMainArea::drawInfoScreenTickNr(tickNr);
            }
            previousBatVoltRaw = batVoltRaw;
            previousSeconds = seconds;
            previousTickNr = tickNr;
        }
    } else {
        displayInitialized = false;
    }
    return true;
}
