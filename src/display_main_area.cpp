#include "display_main_area.h"
#include "gfx/inclinometer_images.h"
#include <iomanip>
#include <sstream>

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

    std::ostringstream ss1;
    ss1 << std::setfill(' ') << std::left << std::setw(3) << pitchAbsInt << ' ';
    std::ostringstream ss2;
    ss2 << std::setfill(' ') << std::setw(3) << rollAbsInt << ' ';

    tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
    tft.setFreeFont(&FreeMono18pt7b);
    tft.drawString(ss1.str().c_str(), 8, 44, GFXFF);
    tft.drawString(ss2.str().c_str(), 248, 44, GFXFF);

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
    std::ostringstream ss;
    ss << std::setfill(' ') << std::left << std::setw(5) << value << ' ';
    tft.setTextColor(textColor, TFT_BLACK, true);
    tft.setFreeFont(&FreeMono9pt7b);
    tft.drawString(ss.str().c_str(), x, y, GFXFF);
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
    std::ostringstream ss;
    tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
    tft.setFreeFont(&FreeMono24pt7b);
    tft.drawString("      km/h", 0, 60, GFXFF);
    tft.setFreeFont(&FreeMono9pt7b);
    tft.drawString("      km", 100, 120, GFXFF);
    tft.drawString("Reset km", 260, 140, FONT2);
}

void DisplayMainArea::drawSpeed(double speedKmh, double distanceMeters, unsigned long tickNr) {
    int scaleSpeed = abs(static_cast<int>(speedKmh * 10.0));
    int scaleDistance = abs(static_cast<int>(distanceMeters / 100.0));

    std::ostringstream ss1;
    ss1 << std::setfill(' ') << std::right << std::setw(5) << scaleSpeed << ' ';
    tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
    tft.setFreeFont(&FreeMono24pt7b);
    tft.drawString(ss1.str().c_str(), 0, 60, GFXFF);

    std::ostringstream ss2;
    ss2 << std::setfill(' ') << std::right << std::setw(5) << scaleDistance << ' ';
    tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
    tft.setFreeFont(&FreeMono9pt7b);
    tft.drawString(ss2.str().c_str(), 100, 120, GFXFF);

    // drawNumber(static_cast<int>(distanceMeters));
}

void DisplayMainArea::drawInfoScreenInitial() {
    tft.setTextColor(TFT_SKYBLUE, TFT_BLACK, true);
    tft.setFreeFont(&FreeMono9pt7b);
    tft.drawString("Bat. time: ", 20, 60, GFXFF);
    tft.drawString("Bat. sensor: ", 20, 80, GFXFF);
    tft.drawString("Speed sensor: ", 20, 100, GFXFF);
    tft.drawString("Distance (m): ", 20, 120, GFXFF);
    tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
    tft.drawString("Reset bat.", 260, 140, FONT2);
}

void DisplayMainArea::drawInfoScreenBatSensor(uint16_t batVoltRaw) {
    std::ostringstream ss;
    ss << batVoltRaw << "    ";
    tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
    tft.setFreeFont(&FreeMono9pt7b);
    tft.drawString(ss.str().c_str(), 163, 80, GFXFF);
}

void DisplayMainArea::drawInfoScreenBatTime(uint16_t batteryMinutes) {
    auto hours = batteryMinutes / 60;
    batteryMinutes %= 60;

    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(2) << hours << ':'
       << std::setfill('0') << std::setw(2) << batteryMinutes << ' ';

    tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
    tft.setFreeFont(&FreeMono9pt7b);
    tft.drawString(ss.str().c_str(), 152, 60, GFXFF);
}

void DisplayMainArea::drawInfoScreenDistance(double distance) {
    std::ostringstream ss;
    ss << static_cast<int>(distance) << "          ";
    tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
    tft.setFreeFont(&FreeMono9pt7b);
    tft.drawString(ss.str().c_str(), 174, 120, GFXFF);
}

void DisplayMainArea::drawInfoScreenTickNr(unsigned long tickNr) {
    std::ostringstream ss;
    ss << std::setfill(' ') << std::left << std::setw(6) << tickNr << ' ';

    tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
    tft.setFreeFont(&FreeMono9pt7b);
    tft.drawString(ss.str().c_str(), 174, 100, GFXFF);
}
