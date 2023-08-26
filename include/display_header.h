#ifndef DISPLAYHEADER_H
#define DISPLAYHEADER_H

class DisplayHeader {
public:
    static void init();
    static void printText(const char* str);
    static void drawBatteryOutline();
    static void drawBatteryIndicator(double percentage);
    static void drawBluetoothImage();
    static void drawBluetoothConnectedImage();
    static void drawBluetoothDisabledImage();
    static void drawBluetoothSearchingImage();
};

#endif //DISPLAYHEADER_H
