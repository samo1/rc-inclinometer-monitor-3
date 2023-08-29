#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <BLEDevice.h>
#include "task_scheduler.h"

static BLEUUID infoServiceUUID("be75903a-14b3-11ec-a7df-e069953c4ba2");
static BLEUUID pitchRollCharUUID("d6c77054-14b3-11ec-b16c-e069953c4ba2");

class Bluetooth;

class BluetoothReconnectTask : public Task {
public:
    explicit BluetoothReconnectTask(Bluetooth &bluetooth) : Task(0, TASK_ONCE, &scheduler, false), bluetooth(bluetooth) {
        setSchedulingOption(TASK_INTERVAL);
    }
    bool Callback() override;
private:
    Bluetooth& bluetooth;
};

class Bluetooth : public BLEAdvertisedDeviceCallbacks, public BLEClientCallbacks {
public:
    Bluetooth() : bluetoothReconnectTask(*this) {}
    friend class BluetoothReconnectTask;

    void initialize();
    void loop();
    void onResult(BLEAdvertisedDevice advertisedDevice) override;
    void onConnect(BLEClient *pClient) override;
    void onDisconnect(BLEClient *pClient) override;

    bool isConnected();
    static double getPitch();
    static double getRoll();

private:
    BLEAdvertisedDevice *serverDevice = nullptr;
    BLEClient *client = nullptr;
    bool doConnect = false;
    bool connectInProgress = false;
    bool connected = false;
    bool doReconnect = false;
    BLERemoteCharacteristic *pitchRollChar = nullptr;

    BluetoothReconnectTask bluetoothReconnectTask;

    void startScan();
    bool connect();
    void disconnect();
};

#endif //BLUETOOTH_H
