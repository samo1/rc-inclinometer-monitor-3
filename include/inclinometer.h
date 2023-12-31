#ifndef INCLINOMETER_H
#define INCLINOMETER_H

#include "bluetooth.h"
#include "display_main_area.h"
#include "task_scheduler.h"

class Inclinometer : public Task {
public:
    Inclinometer(StateManager& stateManager, DisplayMainArea& displayMainArea)
            : Task(200, TASK_FOREVER, &scheduler, false),
              stateManager(stateManager),
              displayMainArea(displayMainArea) {
        setSchedulingOption(TASK_INTERVAL);
    }

    bool Callback() override;
    bool isWarning();

private:
    StateManager& stateManager;
    DisplayMainArea& displayMainArea;
    double previousPitch = 0.0;
    double previousRoll = 0.0;
    bool warning = false;
};

#endif //INCLINOMETER_H
