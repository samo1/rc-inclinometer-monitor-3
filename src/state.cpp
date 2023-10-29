#include "state.h"
#include "display_header.h"

State StateManager::getState() {
    return currentState;
}

void StateManager::setState(State state) {
    if (state == State::disconnected) {
        stateToReturn = currentState;
        currentState = state;
    } else if (state == State::connected) {
        currentState = state;
        updateDisplayHeader();
        currentState = stateToReturn;
    } else {
        currentState = state;
    }
    stateChanged = true;
    updateDisplayHeader();
}

bool StateManager::stateChangedNow() {
    if (stateChanged) {
        stateChanged = false;
        return true;
    }
    return false;
}

void StateManager::goToNextState() {
    switch (currentState) {
        case State::initial:
            currentState = State::inclinometer;
            stateChanged = true;
            break;
        case State::connecting:
        case State::connected:
        case State::disconnected:
            // do nothing
            break;
        case State::inclinometer:
            currentState = State::winch;
            stateChanged = true;
            break;
        case State::winch:
            currentState = State::dig;
            stateChanged = true;
            break;
        case State::dig:
            currentState = State::inclinometer;
            stateChanged = true;
            break;
    }
    updateDisplayHeader();
}

void StateManager::updateDisplayHeader() {
    switch (currentState) {
        case State::initial:
            // do nothing
            break;
        case State::connecting:
            DisplayHeader::printText("Connecting...");
            DisplayHeader::drawBluetoothSearchingImage();
            break;
        case State::connected:
            DisplayHeader::printText("Connected");
            DisplayHeader::drawBluetoothConnectedImage();
            break;
        case State::disconnected:
            DisplayHeader::printText("Disconnected");
            DisplayHeader::drawBluetoothDisabledImage();
            break;
        case State::inclinometer:
            DisplayHeader::printText("Inclinometer");
            break;
        case State::winch:
            DisplayHeader::printText("Winch");
            break;
        case State::dig:
            DisplayHeader::printText("Dig");
            break;
    }
}
