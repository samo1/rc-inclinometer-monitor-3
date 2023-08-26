#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <BLEDevice.h>

static BLEUUID infoServiceUUID("be75903a-14b3-11ec-a7df-e069953c4ba2");
static BLEUUID pitchRollCharUUID("d6c77054-14b3-11ec-b16c-e069953c4ba2");

class Bluetooth : public BLEAdvertisedDeviceCallbacks, public BLEClientCallbacks {
public:
    void initialize();
    void loop();
    void onResult(BLEAdvertisedDevice advertisedDevice) override;
    void onConnect(BLEClient *pClient) override;
    void onDisconnect(BLEClient *pClient) override;

private:
    BLEAdvertisedDevice *serverDevice;
    BLEClient *client = nullptr;
    bool doConnect = false;
    bool connected = false;
    BLERemoteCharacteristic *pitchRollChar;
    bool connect();
    void disconnect();
};

#endif //BLUETOOTH_H
