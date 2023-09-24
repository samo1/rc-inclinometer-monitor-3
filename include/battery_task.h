#ifndef BATTERY_TASK_H
#define BATTERY_TASK_H

#include "task_scheduler.h"

class BatteryTask : public Task {
public:
    explicit BatteryTask() : Task(5000, TASK_FOREVER, &scheduler, false) {
        setSchedulingOption(TASK_INTERVAL);
    }
    bool Callback() override;
    void measureAndDrawBatteryIndicator();
    double getBatteryPercentage() const;
private:
    double batteryPercentage = 0.0;
    static double measureBatteryPercentage();
};

#endif //BATTERY_TASK_H
