#include "inclinometer.h"
#include "display_header.h"
#include "display_main_area.h"

bool Inclinometer::Callback() {
    if (showOnScreen) {
        double pitch = Bluetooth::getPitch();
        double roll = Bluetooth::getRoll();
        warning = (pitch >= 45.0 || pitch <= -45.0 || roll >= 25 || roll <= -25);
        if (pitch != previousPitch || roll != previousRoll) {
            displayMainArea.drawInclinometer(pitch, roll);
            previousPitch = pitch;
            previousRoll = roll;
        }
    }

    if (bluetooth.isConnected()) {
        if (!showOnScreen) {
            showOnScreen = true;
            DisplayHeader::printText("Inclinometer");
        }
    } else {
        showOnScreen = false;
    }
    return true;
}

bool Inclinometer::isWarning() {
    return warning;
}
