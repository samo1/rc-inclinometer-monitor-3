#include "battery_task.h"
#include "display_header.h"
#include "display_main_area.h"
#include <Arduino.h>

bool BatteryTask::Callback() {
    measureAndDrawBatteryIndicator();
    return true;
}

void BatteryTask::measureAndDrawBatteryIndicator() {
    batteryPercentage = measureBatteryPercentage();
    DisplayHeader::drawBatteryIndicator(batteryPercentage);
}

double BatteryTask::measureBatteryPercentage() {
    uint16_t rawValue = analogRead(BAT_VOLT);

    //DisplayMainArea::drawNumber(rawValue);
    // USB power = 2835
    // battery 4.05 = 2409;2452
    //         3.92 = 2340
    //         3.80 = 2266;2221
    //         3.77 = 2253;2221
    //         3.71 = 2233;2210
    //         3.70 = 2223;2203
    //         (4.2V could be cca 2500?)

    double percentage = (rawValue - 2200.0) / (2450.0 - 2200.0);

    //double batteryVoltage = (rawValue * 2 * 3.3) / 4096;
    //double percentage = (batteryVoltage - 3.7) / (4.2 - 3.7);
    if (percentage < 0) {
        percentage = 0;
    }
    if (percentage > 1) {
        percentage = 1;
    }
    return percentage;
}

double BatteryTask::getBatteryPercentage() const {
    return batteryPercentage;
}
