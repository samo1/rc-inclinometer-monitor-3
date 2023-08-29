#include "display_main_area.h"

#include "png_images.h"
#include "lcd.h"
#include <iomanip>
#include <sstream>

void DisplayMainArea::init() {
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
}
