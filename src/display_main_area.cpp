#include "display_main_area.h"
#include "gfx/inclinometer_images.h"
#include <cstdio>

void DisplayMainArea::init() {
    clear();

    jeepPitchSprite.createSprite(100, 100);
    jeepPitchSprite.setSwapBytes(true);
    jeepPitchSprite.pushImage(0, 0, 100, 100, reinterpret_cast<const uint16_t *>(inclinometer_jeep_pitch_rgb565));
    jeepPitchSprite.setPivot(50, 50);

    jeepRollSprite.createSprite(100, 100);
    jeepRollSprite.setSwapBytes(true);
    jeepRollSprite.pushImage(0, 0, 100, 100, reinterpret_cast<const uint16_t *>(inclinometer_jeep_roll_rgb565));
    jeepRollSprite.setPivot(50, 50);
}

void DisplayMainArea::clear() {
    tft.fillRect(0, 36, 320, 170 - 36, TFT_BLACK);
}

void DisplayMainArea::drawInclinometer(double pitch, double roll) {
    int pitchAbsInt = abs(static_cast<int>(pitch));
    int rollAbsInt = abs(static_cast<int>(roll));

    char buf1[8];
    char buf2[8];
    snprintf(buf1, sizeof(buf1), "%-3d ", pitchAbsInt);
    snprintf(buf2, sizeof(buf2), "%3d ", rollAbsInt);

    tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
    tft.setFreeFont(&FreeMono18pt7b);
    tft.drawString(buf1, 8, 44, GFXFF);
    tft.drawString(buf2, 248, 44, GFXFF);

    auto pitchAngle = static_cast<int16_t>(180.0 + pitch);
    auto rollAngle = static_cast<int16_t>(180.0 - roll);

    tft.setPivot(160 - 68, 115);
    jeepPitchSprite.pushRotated(pitchAngle, TFT_BLACK);
    tft.setPivot(160 + 68, 115);
    jeepRollSprite.pushRotated(rollAngle, TFT_BLACK);
}

void DisplayMainArea::drawInclinometerButtonHelp(bool soundEnabled) {
    if (soundEnabled) {
        tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
        tft.drawString("Snd-", 290, 140, FONT2);
    } else {
        tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
        tft.drawString("Snd+", 290, 140, FONT2);
    }
}

void DisplayMainArea::drawNumber(uint16_t value, int32_t x = 0, int32_t y = 154, uint16_t textColor = TFT_DARKGREY) {
    char buf[8];
    snprintf(buf, sizeof(buf), "%-5u ", value);
    tft.setTextColor(textColor, TFT_BLACK, true);
    tft.setFreeFont(&FreeMono9pt7b);
    tft.drawString(buf, x, y, GFXFF);
}

void DisplayMainArea::drawWinch(bool enabled, char movement) {
    if (enabled) {
        tft.setTextColor(TFT_GREENYELLOW, TFT_BLACK, true);
        tft.drawString("Winch ON   ", 90, 60, FONT4);
    } else {
        tft.setTextColor(TFT_RED, TFT_BLACK, true);
        tft.drawString("Winch OFF", 90, 60, FONT4);
    }
    if (movement == 'S') {
        tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK, true);
        tft.drawString("stopped          ", 90, 94, FONT4);
    } else if (movement == 'I') {
        tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK, true);
        tft.drawString("winching in   ", 90, 94, FONT4);
    } else if (movement == 'O') {
        tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK, true);
        tft.drawString("winching out", 90, 94, FONT4);
    }
}

void DisplayMainArea::drawWinchButtonHelp(bool enabled) {
    if (enabled) {
        tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
        tft.drawString("Turn off", 260, 140, FONT2);
    } else {
        tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
        tft.drawString("Turn on  ", 260, 140, FONT2);
    }
}

void DisplayMainArea::drawDig(bool enabled) {
    if (enabled) {
        tft.setTextColor(TFT_ORANGE, TFT_BLACK, true);
        tft.drawString("      Front dig      ", 80, 60, FONT4);
    } else {
        tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK, true);
        tft.drawString("All wheel drive", 80, 60, FONT4);
    }
}

void DisplayMainArea::drawDigButtonHelp(bool enabled) {
    if (enabled) {
        tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
        tft.drawString("Turn off", 260, 140, FONT2);
    } else {
        tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
        tft.drawString("Turn on  ", 260, 140, FONT2);
    }
}

void DisplayMainArea::drawSpeedInitial() {
    tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
    tft.setFreeFont(&FreeMono24pt7b);
    tft.drawString("      km/h", 0, 60, GFXFF);
    tft.setFreeFont(&FreeMono9pt7b);
    tft.drawString("      km", 100, 120, GFXFF);
}

void DisplayMainArea::drawSpeed(double speedKmh, double distanceMeters, unsigned long tickNr) {
    int scaleSpeed = abs(static_cast<int>(speedKmh * 10.0));
    int scaleDistance = abs(static_cast<int>(distanceMeters / 100.0));

    char buf1[8];
    char buf2[8];
    snprintf(buf1, sizeof(buf1), "%5d ", scaleSpeed);
    snprintf(buf2, sizeof(buf2), "%5d ", scaleDistance);

    tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
    tft.setFreeFont(&FreeMono24pt7b);
    tft.drawString(buf1, 0, 60, GFXFF);

    tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
    tft.setFreeFont(&FreeMono9pt7b);
    tft.drawString(buf2, 100, 120, GFXFF);
}

void DisplayMainArea::drawInfoScreenInitial() {
    tft.setTextColor(TFT_SKYBLUE, TFT_BLACK, true);
    tft.setFreeFont(&FreeMono9pt7b);
    tft.drawString("Bat. time: ", 20, 55, GFXFF);
    tft.drawString("Bat. sensor: ", 20, 75, GFXFF);
    tft.drawString("Wheel size: ", 20, 95, GFXFF);
    tft.drawString("Speed sensor: ", 20, 115, GFXFF);
    tft.drawString("Distance (m): ", 20, 135, GFXFF);
    tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
    tft.setFreeFont(&FreeMono9pt7b);
    tft.drawString("Set", 290, 140, FONT2);
}

void DisplayMainArea::drawInfoScreenBatSensor(uint16_t batVoltRaw) {
    char buf[16];
    snprintf(buf, sizeof(buf), "%u    ", batVoltRaw);
    tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
    tft.setFreeFont(&FreeMono9pt7b);
    tft.drawString(buf, 163, 75, GFXFF);
}

void DisplayMainArea::drawInfoScreenBatTime(uint16_t batteryMinutes) {
    auto hours = batteryMinutes / 60;
    batteryMinutes %= 60;

    char buf[16];
    snprintf(buf, sizeof(buf), "%02u:%02u ", hours, batteryMinutes);

    tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
    tft.setFreeFont(&FreeMono9pt7b);
    tft.drawString(buf, 141, 55, GFXFF);
}

void DisplayMainArea::drawInfoScreenDistance(double distance) {
    char buf[24];
    snprintf(buf, sizeof(buf), "%d          ", static_cast<int>(distance));
    tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
    tft.setFreeFont(&FreeMono9pt7b);
    tft.drawString(buf, 174, 135, GFXFF);
}

void DisplayMainArea::drawInfoScreenTickNr(unsigned long tickNr) {
    char buf[16];
    snprintf(buf, sizeof(buf), "%-6lu ", tickNr);

    tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
    tft.setFreeFont(&FreeMono9pt7b);
    tft.drawString(buf, 174, 115, GFXFF);
}

void DisplayMainArea::drawInfoScreenDpr(double mmDistancePerRevolution) {
    char buf[24];
    snprintf(buf, sizeof(buf), "%.1f          ", mmDistancePerRevolution);
    tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
    tft.setFreeFont(&FreeMono9pt7b);
    tft.drawString(buf, 152, 95, GFXFF);
}

void DisplayMainArea::drawInfoScreenSelection(InfoScreenSelection selection) {
    tft.setTextColor(TFT_GOLD, TFT_BLACK, true);
    tft.setFreeFont(&FreeMono9pt7b);
    switch (selection) {
        case InfoScreenSelection::NONE:
            tft.drawString("  ", 5, 55, GFXFF);
            tft.drawString("  ", 5, 75, GFXFF);
            tft.drawString("  ", 5, 95, GFXFF);
            tft.drawString("  ", 5, 115, GFXFF);
            tft.drawString("  ", 5, 135, GFXFF);
            break;
        case InfoScreenSelection::BATTERY:
            tft.drawString("> ", 5, 55, GFXFF);
            tft.drawString("  ", 5, 75, GFXFF);
            tft.drawString("  ", 5, 95, GFXFF);
            tft.drawString("  ", 5, 115, GFXFF);
            tft.drawString("  ", 5, 135, GFXFF);
            break;
        case InfoScreenSelection::WHEEL_SIZE:
            tft.drawString("  ", 5, 55, GFXFF);
            tft.drawString("  ", 5, 75, GFXFF);
            tft.drawString("> ", 5, 95, GFXFF);
            tft.drawString("  ", 5, 115, GFXFF);
            tft.drawString("  ", 5, 135, GFXFF);
            break;
        case InfoScreenSelection::DISTANCE:
            tft.drawString("  ", 5, 55, GFXFF);
            tft.drawString("  ", 5, 75, GFXFF);
            tft.drawString("  ", 5, 95, GFXFF);
            tft.drawString("  ", 5, 115, GFXFF);
            tft.drawString("> ", 5, 135, GFXFF);
            break;
    }
}
