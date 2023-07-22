#include "lcd_power_task.h"

bool LcdPowerTask::Callback() {
    turnLcdOff();
    return false;
}

bool LcdPowerTask::OnEnable() {
    return true;
}

void LcdPowerTask::turnLcdOn() {
    ledcSetup(LEDC_CHANNEL_0, 2000, 8);
    ledcAttachPin(LCD_BL, LEDC_CHANNEL_0);
    ledcWrite(LEDC_CHANNEL_0, MAX_BRIGHNESS);
    // digitalWrite(LCD_POWER_ON, HIGH);
}

void LcdPowerTask::turnLcdOff() {
    ledcSetup(LEDC_CHANNEL_0, 2000, 8);
    ledcAttachPin(LCD_BL, LEDC_CHANNEL_0);
    ledcWrite(LEDC_CHANNEL_0, MIN_BRIGHNESS);
    // digitalWrite(LCD_POWER_ON, LOW);
}

bool LcdPowerTask::turnLcdOffAfterDelay() {
    return restartDelayed(DELAY_MS);
}
