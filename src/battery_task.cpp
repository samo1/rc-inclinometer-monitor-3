#include "battery_task.h"
#include "debug.h"
#include "display_header.h"
#include "display_main_area.h"
#include "preferences_enums.h"
#include <Arduino.h>

bool BatteryTask::Callback() {
    measureAndDrawBatteryIndicator();
    return true;
}

void BatteryTask::measureAndDrawBatteryIndicator() {
    if (!previousBatteryTimeInitialized) {
        readStoredBatteryTime();
        previousBatteryTimeMinutes = storedBatteryTimeMinutes;
        previousBatteryTimeInitialized = true;
    }
    rawValue = analogRead(BAT_VOLT);
    uint16_t minutesSinceStartup = (millis() - startupTime) / 60000;
    batteryTimeMinutes = previousBatteryTimeMinutes + minutesSinceStartup;
    storeCurrentBatteryTime();
    calculateBatteryPercentage();
    DisplayHeader::drawBatteryIndicator(batteryPercentage);
}

void BatteryTask::calculateBatteryPercentage() {
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
    //         full = 3295 - 3333
    //         40 min = 2642 - 3290
    //         60 min = 2642 - 3290
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
    if (rawValue < 2400) {
        percentage = 0;
    } else {
        double remainingMinutes = maxBatteryTimeMinutes - batteryTimeMinutes;
        percentage = remainingMinutes / maxBatteryTimeMinutes;
    }

    // double batteryVoltage = (rawValue - 550.0) / 440.0;

    //double batteryVoltage = (rawValue * 2 * 3.3) / 4096;
    //double percentage = (batteryVoltage - 3.7) / (4.2 - 3.7);
    if (percentage < 0) {
        percentage = 0;
    }
    if (percentage > 1) {
        percentage = 1;
    }

    batteryPercentage = percentage;
}

void BatteryTask::readStoredBatteryTime() {
    DEBUG_PRINTLN("Reading battery time");
    bool success = preferences.begin(PREFERENCES_NAME, true);
    if (!success) {
        DEBUG_PRINTLN("Failed to open preferences");
    }
    storedBatteryTimeMinutes = preferences.getUShort(PREFERENCES_BATTETY_TIME_MINUTES);
    preferences.end();
    batteryTimeMinutes = storedBatteryTimeMinutes;
    DEBUG_PRINT("Battery time read: ");
    DEBUG_PRINTLN(storedBatteryTimeMinutes);
}

void BatteryTask::storeCurrentBatteryTime() {
    if (batteryTimeMinutes != storedBatteryTimeMinutes) {
        DEBUG_PRINT("Writing battery time: ");
        DEBUG_PRINTLN(batteryTimeMinutes);
        bool success = preferences.begin(PREFERENCES_NAME, false);
        if (!success) {
            DEBUG_PRINTLN("Failed to open preferences");
        }
        preferences.putUShort(PREFERENCES_BATTETY_TIME_MINUTES, batteryTimeMinutes);
        preferences.end();
        storedBatteryTimeMinutes = batteryTimeMinutes;
    }
}

void BatteryTask::resetBatteryTime() {
    previousBatteryTimeMinutes = 0;
    startupTime = millis();
    batteryTimeMinutes = 0;
    storeCurrentBatteryTime();
}

double BatteryTask::getBatteryPercentage() const {
    return batteryPercentage;
}

uint16_t BatteryTask::getRawValue() const {
    return rawValue;
}

uint16_t BatteryTask::getBatteryTimeMinutes() const {
    return batteryTimeMinutes;
}
