#ifndef LCD_POWER_TASK_H
#define LCD_POWER_TASK_H

#include <TaskSchedulerDeclarations.h>
#include "lcd.h"

#define DELAY_MS 30000
#define MIN_BRIGHNESS 0
#define MAX_BRIGHNESS 255
#define LEDC_CHANNEL_0 0

class PowerSavingTask : public Task {
public:
    explicit PowerSavingTask(Scheduler* s) : Task(0, TASK_ONCE, s, false) {
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
