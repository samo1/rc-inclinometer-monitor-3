#ifndef INCLINOMETER_H
#define INCLINOMETER_H

#include "bluetooth.h"
#include "display_main_area.h"
#include "task_scheduler.h"

class Inclinometer : public Task {
public:
    explicit Inclinometer(Bluetooth &bluetooth, DisplayMainArea &displayMainArea)
            : Task(200, TASK_FOREVER, &scheduler, false),
              bluetooth(bluetooth),
              displayMainArea(displayMainArea) {
        setSchedulingOption(TASK_INTERVAL);
    }

    bool Callback() override;
    bool isWarning();

private:
    Bluetooth& bluetooth;
    DisplayMainArea& displayMainArea;
    bool showOnScreen = false;
    double previousPitch = 0.0;
    double previousRoll = 0.0;
    bool warning = false;
};

#endif //INCLINOMETER_H
