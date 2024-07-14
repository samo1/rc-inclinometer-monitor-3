#include "speed.h"

#define BLE_CMD_RESET_SPEED "reset_speed"

bool Speed::Callback() {
    if (stateManager.getState() == State::speed) {
        double speed = Bluetooth::getSpeed();
        double distance = Bluetooth::getDistance();
        unsigned long tickNr = Bluetooth::getTickNr();
        if (speed != previousSpeed || distance != previousDistance || tickNr != previousTickNr
            || stateManager.stateChangedNow()) {
            if (!displayInitialized) {
                DisplayMainArea::drawSpeedInitial();
                displayInitialized = true;
            }
            DisplayMainArea::drawSpeed(speed, distance, tickNr);
            previousSpeed = speed;
            previousDistance = distance;
            previousTickNr = tickNr;
        }
    } else {
        displayInitialized = false;
    }
    return true;
}

void Speed::resetTotalDistance() {
    if (stateManager.getState() == State::speed) {
        std::string cmd(BLE_CMD_RESET_SPEED);
        bluetooth.sendCommand(cmd);
    }
}
