// Image files can be converted to arrays using the tool here:
// https://notisrac.github.io/FileToCArray/
// To use this tool:
//   1. Drag and drop PNG image file on "Browse..." button
//   2. Tick box "Treat as binary"
//   3. Click "Convert"
//   4. Click "Save as file" and move the header file to sketch folder
//      (alternatively use the "Copy to clipboard" and paste into a new tab)
//   5. Open the sketch in IDE
//   6. Include the header file containing the array (SpongeBob.h in this example)

#include "png_images.h"
#include "lcd.h"
#include "icons_bt.h"
#include <PNGdec.h>

#define MAX_IMAGE_WIDTH 48 // Sets rendering line buffer lengths, adjust for your images

PNG png;

// Position variables must be global (PNGdec does not handle position coordinates)
int32_t xpos = 0;
int32_t ypos = 0;

//=========================================v==========================================
//  pngDraw: Callback function to draw pixels to the display
//====================================================================================
// This function will be called during decoding of the png file to render each image
// line to the TFT. PNGdec generates the image line and a 1bpp mask.
void pngDraw(PNGDRAW *pDraw) {
    uint16_t lineBuffer[MAX_IMAGE_WIDTH];          // Line buffer for rendering
    uint8_t maskBuffer[1 + MAX_IMAGE_WIDTH / 8];  // Mask buffer

    png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);

    if (png.getAlphaMask(pDraw, maskBuffer, 255)) {
        // Note: pushMaskedImage is for pushing to the TFT and will not work pushing into a sprite
        tft.pushMaskedImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer, maskBuffer);
    }
}

void drawImage(int32_t x, int32_t y, const unsigned char* data, int dataSize) {
    xpos = x;
    ypos = y;
    int rc = png.openFLASH((uint8_t *)data, dataSize, pngDraw);
    if (rc == PNG_SUCCESS) {
        tft.startWrite();
        png.decode(nullptr, 0);
        tft.endWrite();
    }
}

namespace PngImages {
    void drawBluetoothImage(int32_t x, int32_t y) {
        drawImage(x, y, baseline_bluetooth_white_24dp, sizeof(baseline_bluetooth_white_24dp));
    }

    void drawBluetoothConnectedImage(int32_t x, int32_t y) {
        drawImage(x, y, baseline_bluetooth_connected_white_24dp, sizeof(baseline_bluetooth_connected_white_24dp));
    }

    void drawBluetoothDisabledImage(int32_t x, int32_t y) {
        drawImage(x, y, baseline_bluetooth_disabled_white_24dp, sizeof(baseline_bluetooth_disabled_white_24dp));
    }

    void drawBluetoothSearchingImage(int32_t x, int32_t y) {
        drawImage(x, y, baseline_bluetooth_searching_white_24dp, sizeof(baseline_bluetooth_searching_white_24dp));
    }
}
