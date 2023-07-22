#include "power_saving_task.h"

bool PowerSavingTask::Callback() {
    turnLcdOff();
    return false;
}

void PowerSavingTask::turnLcdOn() {
    ledcSetup(LEDC_CHANNEL_0, 2000, 8);
    ledcAttachPin(LCD_BL, LEDC_CHANNEL_0);
    ledcWrite(LEDC_CHANNEL_0, MAX_BRIGHNESS);
    // digitalWrite(LCD_POWER_ON, HIGH);
}

void PowerSavingTask::turnLcdOff() {
    ledcSetup(LEDC_CHANNEL_0, 2000, 8);
    ledcAttachPin(LCD_BL, LEDC_CHANNEL_0);
    ledcWrite(LEDC_CHANNEL_0, MIN_BRIGHNESS);
    // digitalWrite(LCD_POWER_ON, LOW);
}

bool PowerSavingTask::turnLcdOffAfterDelay() {
    return restartDelayed(DELAY_MS);
}

void PowerSavingTask::wakeUp() {
    turnLcdOn();
    turnLcdOffAfterDelay();
}
