#pragma once

enum class State {
    initial,
    connecting,
    connected,
    disconnected,
    inclinometer,
    winch,
    dig
};

class StateManager {
public:
    State getState();
    void setState(State state);
    void goToNextState();
    bool stateChangedNow();
private:
    State currentState = State::initial;
    State stateToReturn = State::initial;
    bool stateChanged = false;

    void updateDisplayHeader();
};
