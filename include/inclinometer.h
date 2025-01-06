#ifndef INCLINOMETER_H
#define INCLINOMETER_H

#include "bluetooth.h"
#include "display_main_area.h"
#include "task_scheduler.h"

class Inclinometer : public Task {
public:
    Inclinometer(StateManager& stateManager, DisplayMainArea& displayMainArea, Bluetooth& bluetooth)
            : Task(200, TASK_FOREVER, &scheduler, false),
              stateManager(stateManager),
              displayMainArea(displayMainArea),
              bluetooth(bluetooth) {
        setSchedulingOption(TASK_INTERVAL);
    }

    bool Callback() override;
    void handleButtonClick();
    bool isWarning();

private:
    StateManager& stateManager;
    DisplayMainArea& displayMainArea;
    Bluetooth& bluetooth;

    double previousPitch = 0.0;
    double previousRoll = 0.0;
    bool previousSoundEnabled = false;
    bool warning = false;
};

#endif //INCLINOMETER_H
