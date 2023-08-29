#ifndef INCLINOMETER_H
#define INCLINOMETER_H

#include "bluetooth.h"
#include "task_scheduler.h"

class Inclinometer : public Task {
public:
    explicit Inclinometer(Bluetooth& bluetooth) : Task(500, TASK_FOREVER, &scheduler, false),
                                                  bluetooth(bluetooth) {
        setSchedulingOption(TASK_INTERVAL);
    }

    bool Callback() override;
    bool isWarning();

private:
    Bluetooth& bluetooth;
    bool showOnScreen = false;
    double previousPitch = 0.0;
    double previousRoll = 0.0;
    bool warning = false;
};

#endif //INCLINOMETER_H
