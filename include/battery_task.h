#ifndef BATTERY_TASK_H
#define BATTERY_TASK_H

#include <TaskSchedulerDeclarations.h>
#include "lcd.h"

class BatteryTask : public Task {
public:
    explicit BatteryTask(Scheduler* s) : Task(5000, TASK_FOREVER, s, false) {
        setSchedulingOption(TASK_INTERVAL);
    }
    bool Callback() override;
    void drawBatteryIndicator();
private:
    uint32_t color = TFT_GREEN;
};

#endif //BATTERY_TASK_H
