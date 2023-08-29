#include "battery_task.h"
#include "display_header.h"
#include "display_main_area.h"
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
    uint16_t rawValue = analogRead(BAT_VOLT);

    DisplayMainArea::drawNumber(rawValue); // TODO remove
    // USB power = 2835
    // battery 3.92 = 2340
    //         3.80 = 2266/2221
    //         3.77 = 2253/2221
    //         3.71 = 2233/2210
    //         3.70 = 2223/2203

    double batteryVoltage = (rawValue * 2 * 3.3) / 4096;
    double batteryPercentage = (batteryVoltage - 3.7) / (4.2 - 3.7);
    if (batteryPercentage < 0) {
        batteryPercentage = 0;
    }
    if (batteryPercentage > 1) {
        batteryPercentage = 1;
    }
    return batteryPercentage;
}
