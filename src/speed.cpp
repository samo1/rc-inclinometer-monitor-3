#include "speed.h"

bool Speed::Callback() {
    if (stateManager.getState() == State::speed) {
        double speed = Bluetooth::getSpeed();
        unsigned long tickNr = Bluetooth::getTickNr();
        if (speed != previousSpeed || tickNr != previousTickNr || stateManager.stateChangedNow()) {
            if (!displayInitialized) {
                DisplayMainArea::drawSpeedInitial();
                displayInitialized = true;
            }
            DisplayMainArea::drawSpeed(speed, tickNr);
            previousSpeed = speed;
            previousTickNr = tickNr;
        }
    } else {
        displayInitialized = false;
    }
    return true;
}
