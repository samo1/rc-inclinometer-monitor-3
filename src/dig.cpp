#include "dig.h"

#define BLE_CMD_DIG_ENABLE "dig_enable"
#define BLE_CMD_DIG_DISABLE "dig_disable"

bool Dig::Callback() {
    if (stateManager.getState() == State::dig) {
        bool enabled = Bluetooth::getFrontDigEnabled();
        bool stateChangedNow = stateManager.stateChangedNow();
        if (enabled != this->previousEnabled || stateChangedNow) {
            DisplayMainArea::drawDigButtonHelp(enabled);
            DisplayMainArea::drawDig(enabled);
            this->previousEnabled = enabled;
        }
    }
    return true;
}

void Dig::handleButtonClick() {
    if (stateManager.getState() == State::dig) {
        bool enabled = Bluetooth::getFrontDigEnabled();
        if (enabled) {
            std::string cmd(BLE_CMD_DIG_DISABLE);
            bluetooth.sendCommand(cmd);
        } else {
            std::string cmd(BLE_CMD_DIG_ENABLE);
            bluetooth.sendCommand(cmd);
        }
    }
}
