#pragma once

#include "bluetooth.h"
#include "display_main_area.h"
#include "task_scheduler.h"

class Speed : public Task {
public:
    Speed(StateManager& stateManager, DisplayMainArea& displayMainArea)
            : Task(200, TASK_FOREVER, &scheduler, false),
              stateManager(stateManager),
              displayMainArea(displayMainArea) {
        setSchedulingOption(TASK_INTERVAL);
    }

    bool Callback() override;

private:
    StateManager& stateManager;
    DisplayMainArea& displayMainArea;
    double previousSpeed = 0.0;
    unsigned long previousTickNr = 0;
    boolean displayInitialized = false;
};
