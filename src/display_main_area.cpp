#include "display_main_area.h"

#include "png_images.h"
#include "lcd.h"

void DisplayMainArea::init() {
    tft.fillRect(0, 36, 320, 170 - 36, TFT_BLACK);
}
