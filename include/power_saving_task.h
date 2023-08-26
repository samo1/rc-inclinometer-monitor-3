#ifndef LCD_POWER_TASK_H
#define LCD_POWER_TASK_H

#include "lcd.h"
#include "task_scheduler.h"

class PowerSavingTask : public Task {
public:
    explicit PowerSavingTask() : Task(0, TASK_ONCE, &scheduler, false) {
        setSchedulingOption(TASK_INTERVAL);
        // pinMode(LCD_POWER_ON, OUTPUT);
    }
    bool Callback() override;
    void wakeUp();
private:
    void turnLcdOn();
    void turnLcdOff();
    bool turnLcdOffAfterDelay();
};

#endif //LCD_POWER_TASK_H
