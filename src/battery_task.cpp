#include "battery_task.h"
#include "display_header.h"
#include <Arduino.h>

bool BatteryTask::Callback() {
    measureAndDrawBatteryIndicator();
    return true;
}

void BatteryTask::measureAndDrawBatteryIndicator() {
    double percentage = measureBatteryPercentage();
    DisplayHeader::drawBatteryIndicator(percentage);
}

double BatteryTask::measureBatteryPercentage() {
    double batteryVoltage = (analogRead(BAT_VOLT) * 2 * 3.3) / 4096;
    double batteryPercentage = (batteryVoltage - 3.7) / (4.2 - 3.7);
    if (batteryPercentage < 0) {
        batteryPercentage = 0;
    }
    if (batteryPercentage > 1) {
        batteryPercentage = 1;
    }
    return batteryPercentage;
}
