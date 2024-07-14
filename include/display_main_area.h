#ifndef DISPLAYMAINAREA_H
#define DISPLAYMAINAREA_H

#include "lcd.h"

class DisplayMainArea {
public:
    void init();
    static void clear();
    void drawInclinometer(double pitch, double roll);
    static void drawNumber(uint16_t value, int32_t x, int32_t y, uint16_t textColor);
    static void drawWinch(bool enabled, char movement);
    static void drawWinchButtonHelp(bool enabled);
    static void drawDig(bool enabled);
    static void drawDigButtonHelp(bool enabled);
    static void drawSpeedInitial();
    static void drawSpeed(double speedKmh, double distanceMeters, unsigned long tickNr);
    static void drawInfoScreenInitial();
    static void drawInfoScreenBatSensor(uint16_t batVoltRaw);
    static void drawInfoScreenBatTime(uint16_t batteryMinutes);
    static void drawInfoScreenDistance(double distance);
    static void drawInfoScreenTickNr(unsigned long tickNr);
private:
    TFT_eSprite jeepPitchSprite = TFT_eSprite(&tft);
    TFT_eSprite jeepRollSprite = TFT_eSprite(&tft);
};

#endif //DISPLAYMAINAREA_H
