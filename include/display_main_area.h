#ifndef DISPLAYMAINAREA_H
#define DISPLAYMAINAREA_H

#include "lcd.h"

class DisplayMainArea {
public:
    void init();
    static void clear();
    void drawInclinometer(double pitch, double roll);
    static void drawNumber(uint16_t value);
    static void drawWinch(bool enabled, char movement);
    static void drawWinchButtonHelp(bool enabled);
    static void drawDig(bool enabled);
    static void drawDigButtonHelp(bool enabled);
private:
    TFT_eSprite jeepPitchSprite = TFT_eSprite(&tft);
    TFT_eSprite jeepRollSprite = TFT_eSprite(&tft);
};

#endif //DISPLAYMAINAREA_H
