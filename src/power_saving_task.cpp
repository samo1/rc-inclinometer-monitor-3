#include "power_saving_task.h"

#define DELAY_MS 30000
#define MIN_BRIGHNESS 0
#define MAX_BRIGHNESS 255
#define LEDC_CHANNEL_0 0

bool PowerSavingTask::Callback() {
    if (turnLcdOffOnAutoSleep) {
        turnLcdOff();
    }
    return true;
}

void PowerSavingTask::turnLcdOn() {
    ledcSetup(LEDC_CHANNEL_0, 2000, 8);
    ledcAttachPin(LCD_BL, LEDC_CHANNEL_0);
    ledcWrite(LEDC_CHANNEL_0, MAX_BRIGHNESS);
    // digitalWrite(LCD_POWER_ON, HIGH);
    this->lcdIsOff = false;
}

void PowerSavingTask::turnLcdOff() {
    ledcSetup(LEDC_CHANNEL_0, 2000, 8);
    ledcAttachPin(LCD_BL, LEDC_CHANNEL_0);
    ledcWrite(LEDC_CHANNEL_0, MIN_BRIGHNESS);
    // digitalWrite(LCD_POWER_ON, LOW);
    this->lcdIsOff = true;
}

bool PowerSavingTask::turnLcdOffAfterDelay() {
    return restartDelayed(DELAY_MS);
}

void PowerSavingTask::wakeUp() {
    if (lcdIsOff) {
        turnLcdOn();
    }
    turnLcdOffAfterDelay();
}

void PowerSavingTask::sleep() {
    if (!lcdIsOff) {
        turnLcdOff();
    }
}

bool PowerSavingTask::isLcdOff() {
    return lcdIsOff;
}
