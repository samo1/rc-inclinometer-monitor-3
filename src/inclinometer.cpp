#include "inclinometer.h"
#include "display_header.h"

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
            std::string cmd(BLE_CMD_SND_DISABLE);
            bluetooth.sendCommand(cmd);
        } else {
            std::string cmd(BLE_CMD_SND_ENABLE);
            bluetooth.sendCommand(cmd);
        }
    }
}

bool Inclinometer::isWarning() {
    return warning;
}
