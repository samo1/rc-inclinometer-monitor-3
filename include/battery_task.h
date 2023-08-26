#ifndef BATTERY_TASK_H
#define BATTERY_TASK_H

#include <TaskSchedulerDeclarations.h>

class BatteryTask : public Task {
public:
    explicit BatteryTask(Scheduler* s) : Task(5000, TASK_FOREVER, s, false) {
        setSchedulingOption(TASK_INTERVAL);
    }
    bool Callback() override;
    static void measureAndDrawBatteryIndicator();
private:
    static double measureBatteryPercentage();
};

#endif //BATTERY_TASK_H
