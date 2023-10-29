#include "inclinometer.h"
#include "display_header.h"

bool Inclinometer::Callback() {
    if (stateManager.getState() == State::inclinometer) {
        double pitch = Bluetooth::getPitch();
        double roll = Bluetooth::getRoll();
        warning = (pitch >= 45.0 || pitch <= -45.0 || roll >= 25 || roll <= -25);
        if (pitch != this->previousPitch || roll != this->previousRoll || stateManager.stateChangedNow()) {
            displayMainArea.drawInclinometer(pitch, roll);
            this->previousPitch = pitch;
            this->previousRoll = roll;
        }
    } else {
        warning = false;
    }
    return true;
}

bool Inclinometer::isWarning() {
    return warning;
}
