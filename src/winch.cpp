#include "winch.h"

#define BLE_CMD_WINCH_ENABLE "enable"
#define BLE_CMD_WINCH_DISABLE "disable"
#define BLE_CMD_WINCH_STOP "stop"
#define BLE_CMD_WINCH_IN "in"
#define BLE_CMD_WINCH_OUT "out"

bool Winch::Callback() {
    if (stateManager.getState() == State::winch) {
        bool enabled = Bluetooth::getWinchEnabled();
        char movement = Bluetooth::getWinchMovement();
        bool stateChangedNow = stateManager.stateChangedNow();
        if (enabled != this->previousEnabled || movement != this->previousMovement || stateChangedNow) {
            DisplayMainArea::drawWinchButtonHelp(enabled);
            DisplayMainArea::drawWinch(enabled, movement);
            this->previousEnabled = enabled;
            this->previousMovement = movement;
        }
    }
    return true;
}

void Winch::handleButtonClick() {
    if (stateManager.getState() == State::winch) {
        bool enabled = Bluetooth::getWinchEnabled();
        if (enabled) {
            bluetooth.sendCommand(BLE_CMD_WINCH_DISABLE);
        } else {
            bluetooth.sendCommand(BLE_CMD_WINCH_ENABLE);
        }
    }
}
