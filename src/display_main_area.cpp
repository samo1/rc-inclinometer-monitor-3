#include "display_main_area.h"
#include "inclinometer_images.h"
#include <iomanip>
#include <sstream>

void DisplayMainArea::init() {
    tft.fillRect(0, 36, 320, 170 - 36, TFT_BLACK);

    jeepPitchSprite.createSprite(100, 100);
    jeepPitchSprite.setSwapBytes(true);
    jeepPitchSprite.pushImage(0, 0, 100, 100, reinterpret_cast<const uint16_t *>(inclinometer_jeep_pitch_rgb565));
    jeepPitchSprite.setPivot(50, 50);

    jeepRollSprite.createSprite(100, 100);
    jeepRollSprite.setSwapBytes(true);
    jeepRollSprite.pushImage(0, 0, 100, 100, reinterpret_cast<const uint16_t *>(inclinometer_jeep_roll_rgb565));
    jeepRollSprite.setPivot(50, 50);
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

void DisplayMainArea::drawNumber(uint16_t value) {
    std::ostringstream ss;
    ss << std::setfill(' ') << std::left << std::setw(5) << value << ' ';
    tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
    tft.setFreeFont(&FreeMono9pt7b);
    tft.drawString(ss.str().c_str(), 0, 154, GFXFF);
}
