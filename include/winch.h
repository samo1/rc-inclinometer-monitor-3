#pragma once

#include "bluetooth.h"
#include "display_main_area.h"
#include "task_scheduler.h"

class Winch : public Task {
public:
    Winch(StateManager& stateManager, DisplayMainArea& displayMainArea, Bluetooth& bluetooth)
            : Task(200, TASK_FOREVER, &scheduler, false),
              stateManager(stateManager),
              displayMainArea(displayMainArea),
              bluetooth(bluetooth) {
        setSchedulingOption(TASK_INTERVAL);
    }

    bool Callback() override;
    void handleButtonClick();

private:
    StateManager& stateManager;
    DisplayMainArea& displayMainArea;
    Bluetooth& bluetooth;

    bool previousEnabled = false;
    char previousMovement = 'S';
};
