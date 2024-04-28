#include "battery_task.h"
#include "display_header.h"
#include "display_main_area.h"
#include <Arduino.h>

bool BatteryTask::Callback() {
    measureAndDrawBatteryIndicator();
    return true;
}

void BatteryTask::measureAndDrawBatteryIndicator() {
    rawValue = analogRead(BAT_VOLT);
    batteryPercentage = calculateBatteryPercentage(rawValue);
    DisplayHeader::drawBatteryIndicator(batteryPercentage);
}

double BatteryTask::calculateBatteryPercentage(uint16_t rawValue) {
    //DisplayMainArea::drawNumber(rawValue);
    // USB power = 2835 or 2811-2813
    // 1S LiPo battery
    //         4.05 = 2409;2452
    //         3.92 = 2340
    //         3.80 = 2266;2221
    //         3.77 = 2253;2221
    //         3.71 = 2233;2210
    //         3.70 = 2223;2203
    //         (4.2V could be cca 2500?)
    // 4xAA batteries
    //         5.88 = 3292 - 3302
    //         5,5V = 3001 - 3034
    //         5.33 = 2877 - 2927  12:44
    //         5.2  = 2816 - 2833  13:07
    //         5.19 = 2775 - 2813  13:07 blinks red
    //         5.14 = 2727 - 2773  13:28
    //         5.08 = 2667 - 2707  14:00
    //         5.00 = 2620 - 2663  14:35
    //         4.95 = 2611 - 2652  15:03
    //         4.95 = 2488 - 2522  15:18 beeps
    //         4.92 = 2606 - 2638  15:27 beeps
    // 4xAA black Eneloop
    //         3200 20:45
    //         2860 22:05
    //         2723 01:38
    //         2695 01:50
    // 4xAA Xtar Li-om 4158 mAh
    //         full = 3295
    //         40 min = 2642
    //         60 min = the same
    //         110 min = 2586
    //         240 min = 2578
    //         322 min = 2639
    //         423 min = 2355
    //         426 min = 2110 blinks red
    //         432 min = 1871 beeps

    double percentage;
    //if (rawValue < 2500) {
    //    // LiPo
    //    percentage = (rawValue - 2200.0) / (2450.0 - 2200.0);
    //} else {
    //    // 4xAA
    //    percentage = (rawValue - 2600.0) / (3000.0 - 2600.0);
    //}
    // 4xAA
    // percentage = (rawValue - 2600.0) / (3000.0 - 2600.0);

    // 4xAA Xtar Li-om 4158 mAh
    if (rawValue > 2800) {
        percentage = 1;
    } else if (rawValue < 2400) {
        percentage = 0;
    } else {
        double maxTimeMillis = 25200000; // 7h
        double remainingMllis = maxTimeMillis - millis();
        percentage = remainingMllis / maxTimeMillis;
    }

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

uint16_t BatteryTask::getRawValue() const {
    return rawValue;
}
