#include "battery_task.h"

bool BatteryTask::Callback() {
    drawBatteryIndicator();
    return false;
}

void BatteryTask::drawBatteryIndicator() {
    double batteryVoltage = (analogRead(BAT_VOLT) * 2 * 3.3) / 4096;

    double batteryPercentage = (batteryVoltage - 3.7) / (4.2 - 3.7);
    if (batteryPercentage < 0) {
        batteryPercentage = 0;
    }
    if (batteryPercentage > 1) {
        batteryPercentage = 1;
    }

    int batteryIndicatorWidth = static_cast<int>(24 * batteryPercentage);
    tft->fillRect(276 + batteryIndicatorWidth, 16, 24 - batteryIndicatorWidth, 8, TFT_BLACK);
    tft->fillRect(276, 16, batteryIndicatorWidth, 8, color);
}
