#ifndef BATTERY_TASK_H
#define BATTERY_TASK_H

#define _TASK_OO_CALLBACKS

#include <TaskSchedulerDeclarations.h>
#include "pin_config.h"
#include <TFT_eSPI.h>

class BatteryTask : public Task {
public:
    explicit BatteryTask(Scheduler* s, TFT_eSPI* tft) : Task(5000, TASK_FOREVER, s, false) {
        this->tft = tft;
    }
    bool Callback() override;
    void drawBatteryIndicator();
private:
    TFT_eSPI* tft;
    uint32_t color = TFT_GREEN;
};

#endif //BATTERY_TASK_H
