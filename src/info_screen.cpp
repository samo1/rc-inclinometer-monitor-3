#include "info_screen.h"

bool InfoScreen::Callback() {
    if (stateManager.getState() == State::info) {
        uint16_t batVoltRaw = batteryTask.getRawValue();
        if (batVoltRaw != previousBatVoltRaw || stateManager.stateChangedNow()) {
            DisplayMainArea::drawInfoScreen(batVoltRaw);
            previousBatVoltRaw = batVoltRaw;
        }
    }
    return true;
}
