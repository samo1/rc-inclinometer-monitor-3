#ifndef LCD_POWER_TASK_H
#define LCD_POWER_TASK_H

#include <TaskSchedulerDeclarations.h>
#include "lcd.h"

#define LEDC_CHANNEL_0 0
#define MIN_BRIGHNESS 0
#define MAX_BRIGHNESS 252
#define DELAY_MS 10000

class LcdPowerTask : public Task {
public:
    explicit LcdPowerTask(Scheduler* s) : Task(0, TASK_ONCE, s, false) {
        setSchedulingOption(TASK_INTERVAL);
        // pinMode(LCD_POWER_ON, OUTPUT);
    }
    bool turnLcdOffAfterDelay();
    bool Callback() override;
    bool OnEnable() override;
    void turnLcdOn();
    void turnLcdOff();
};

#endif //LCD_POWER_TASK_H
