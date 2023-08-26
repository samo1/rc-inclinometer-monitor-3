#include "display_header.h"

#include "png_images.h"
#include "lcd.h"

#define HEADER_FONT 4
#define HEADER_BG_COLOR TFT_DARKGREY

#define BT_ICON_X 240
#define BT_ICON_Y 6

void DisplayHeader::init() {
    tft.fillRect(0, 0, 320, 36, HEADER_BG_COLOR);
}

void DisplayHeader::printText(const char *str) {
    tft.fillRect(0, 0, 240, 36, HEADER_BG_COLOR);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, HEADER_BG_COLOR, false);
    tft.drawString(str, 16, 8, HEADER_FONT);
}

void DisplayHeader::drawBatteryOutline() {
    tft.fillRect(272, 12, 32, 16, TFT_WHITE);
    tft.fillRect(304, 16, 4, 8, TFT_WHITE);
}

void DisplayHeader::drawBatteryIndicator(double percentage) {
    uint32_t color = TFT_GREEN;
    int batteryIndicatorWidth = static_cast<int>(24 * percentage);
    tft.fillRect(276 + batteryIndicatorWidth, 16, 24 - batteryIndicatorWidth, 8, TFT_BLACK);
    tft.fillRect(276, 16, batteryIndicatorWidth, 8, color);
}

void DisplayHeader::drawBluetoothImage() {
    tft.fillRect(BT_ICON_X, BT_ICON_Y, 24, 24, HEADER_BG_COLOR);
    PngImages::drawBluetoothImage(BT_ICON_X, BT_ICON_Y);
}

void DisplayHeader::drawBluetoothConnectedImage() {
    tft.fillRect(BT_ICON_X, BT_ICON_Y, 24, 24, HEADER_BG_COLOR);
    PngImages::drawBluetoothConnectedImage(BT_ICON_X, BT_ICON_Y);
}

void DisplayHeader::drawBluetoothDisabledImage() {
    tft.fillRect(BT_ICON_X, BT_ICON_Y, 24, 24, HEADER_BG_COLOR);
    PngImages::drawBluetoothDisabledImage(BT_ICON_X, BT_ICON_Y);
}

void DisplayHeader::drawBluetoothSearchingImage() {
    tft.fillRect(BT_ICON_X, BT_ICON_Y, 24, 24, HEADER_BG_COLOR);
    PngImages::drawBluetoothSearchingImage(BT_ICON_X, BT_ICON_Y);
}
