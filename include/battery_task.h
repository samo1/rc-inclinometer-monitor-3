#ifndef BATTERY_TASK_H
#define BATTERY_TASK_H

#include "task_scheduler.h"
#include <Preferences.h>

class BatteryTask : public Task {
public:
    explicit BatteryTask(Preferences &preferences)
            : Task(5000, TASK_FOREVER, &scheduler, false),
              preferences(preferences) {
        setSchedulingOption(TASK_INTERVAL);
    }
    bool Callback() override;
    void measureAndDrawBatteryIndicator();
    double getBatteryPercentage() const;
    uint16_t getRawValue() const;
    uint16_t getBatteryTimeMinutes() const;
    void resetBatteryTime();
private:
    Preferences &preferences;
    uint16_t rawValue = 0;
    double batteryPercentage = 0.0;
    unsigned long startupTime = millis();
    bool previousBatteryTimeInitialized = false;
    uint16_t previousBatteryTimeMinutes = 0;
    uint16_t batteryTimeMinutes = 0;
    uint16_t storedBatteryTimeMinutes = 0;
    uint16_t maxBatteryTimeMinutes = 420; // 7h
    void calculateBatteryPercentage();
    void readStoredBatteryTime();
    void storeCurrentBatteryTime();
};

#endif //BATTERY_TASK_H
