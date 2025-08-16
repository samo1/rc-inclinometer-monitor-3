#pragma once

#include "bluetooth.h"
#include "display_main_area.h"
#include "state.h"
#include "task_scheduler.h"

class Speed : public Task {
public:
    Speed(StateManager& stateManager, DisplayMainArea& displayMainArea, Bluetooth& bluetooth)
            : Task(200, TASK_FOREVER, &scheduler, false),
              stateManager(stateManager),
              displayMainArea(displayMainArea),
              bluetooth(bluetooth) {
        setSchedulingOption(TASK_INTERVAL);
    }

    bool Callback() override;

private:
    StateManager& stateManager;
    DisplayMainArea& displayMainArea;
    Bluetooth& bluetooth;
    double previousSpeed = 0.0;
    double previousDistance = 0.0;
    unsigned long previousTickNr = 0;
    boolean displayInitialized = false;
};
