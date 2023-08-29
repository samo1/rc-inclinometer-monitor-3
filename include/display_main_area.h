#ifndef DISPLAYMAINAREA_H
#define DISPLAYMAINAREA_H

#include "lcd.h"

class DisplayMainArea {
public:
    void init();
    void drawInclinometer(double pitch, double roll);
    static void drawNumber(uint16_t value);
private:
    TFT_eSprite jeepPitchSprite = TFT_eSprite(&tft);
    TFT_eSprite jeepRollSprite = TFT_eSprite(&tft);
};

#endif //DISPLAYMAINAREA_H
