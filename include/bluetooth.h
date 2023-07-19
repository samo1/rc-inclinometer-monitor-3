#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <BLEDevice.h>

static BLEUUID infoServiceUUID("be75903a-14b3-11ec-a7df-e069953c4ba2");
static BLEUUID pitchRollCharUUID("d6c77054-14b3-11ec-b16c-e069953c4ba2");

class Bluetooth : public BLEAdvertisedDeviceCallbacks, public BLEClientCallbacks {
public:
    void initialize();
    void onResult(BLEAdvertisedDevice advertisedDevice) override;
    void onConnect(BLEClient *pClient) override;
    void onDisconnect(BLEClient *pClient) override;

private:
    BLEAdvertisedDevice* myDevice;
    bool doConnect = false;
    bool connected = false;
    bool doScan = false;
};

#endif //BLUETOOTH_H
