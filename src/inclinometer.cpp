#include "inclinometer.h"

#define BLE_CMD_SND_ENABLE "snd_enable"
#define BLE_CMD_SND_DISABLE "snd_disable"

bool Inclinometer::Callback() {
    if (stateManager.getState() == State::inclinometer) {
        double pitch = Bluetooth::getPitch();
        double roll = Bluetooth::getRoll();
        warning = (pitch >= 45.0 || pitch <= -45.0 || roll >= 25 || roll <= -25);
        bool stateChangedNow = stateManager.stateChangedNow();
        if (pitch != this->previousPitch || roll != this->previousRoll || stateChangedNow) {
            displayMainArea.drawInclinometer(pitch, roll);
            this->previousPitch = pitch;
            this->previousRoll = roll;
        }
        bool soundEnabled = Bluetooth::getSoundEnabled();
        if (soundEnabled != this->previousSoundEnabled || stateChangedNow) {
            DisplayMainArea::drawInclinometerButtonHelp(soundEnabled);
            this->previousSoundEnabled = soundEnabled;
        }
    } else {
        warning = false;
    }
    return true;
}

void Inclinometer::handleButtonClick() {
    if (stateManager.getState() == State::inclinometer) {
        bool soundEnabled = Bluetooth::getSoundEnabled();
        if (soundEnabled) {
            bluetooth.sendCommand(BLE_CMD_SND_DISABLE);
        } else {
            bluetooth.sendCommand(BLE_CMD_SND_ENABLE);
        }
    }
}

bool Inclinometer::isWarning() {
    return warning;
}
