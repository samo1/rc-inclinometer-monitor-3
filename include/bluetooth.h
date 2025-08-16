#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <BLEDevice.h>
#include "state.h"
#include "task_scheduler.h"

static BLEUUID infoServiceUUID("be75903a-14b3-11ec-a7df-e069953c4ba2");
static BLEUUID pitchRollCharUUID("d6c77054-14b3-11ec-b16c-e069953c4ba2");
static BLEUUID winchInfoCharUUID("8a3253c2-758b-11ee-99b9-e069953c4ba2");
static BLEUUID winchControlCharUUID("4eae0d40-699f-11ec-b55b-e069953c4ba2");
static BLEUUID speedCharUUID("c7de114e-9aab-11ee-9c1f-e069953c4ba2");

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
    explicit Bluetooth(StateManager& stateManager) : stateManager(stateManager), bluetoothReconnectTask(*this) {}
    friend class BluetoothReconnectTask;

    void initialize();
    void loop();
    void onResult(BLEAdvertisedDevice advertisedDevice) override;
    void onConnect(BLEClient *pClient) override;
    void onDisconnect(BLEClient *pClient) override;
    void sendCommand(std::string& cmd);

    static double getPitch();
    static double getRoll();
    static bool getWinchEnabled();
    static char getWinchMovement();
    static bool getFrontDigEnabled();
    static bool getSoundEnabled();
    static double getmmDistancePerRevolution();
    static double getSpeed();
    static double getDistance();
    static unsigned long getTickNr();

private:
    StateManager& stateManager;
    BLEAdvertisedDevice *serverDevice = nullptr;
    BLEClient *client = nullptr;
    bool doConnect = false;
    bool connectInProgress = false;
    bool connected = false;
    bool doReconnect = false;
    BLERemoteCharacteristic *pitchRollChar = nullptr;
    BLERemoteCharacteristic *winchInfoChar = nullptr;
    BLERemoteCharacteristic *winchControlChar = nullptr;
    BLERemoteCharacteristic *speedChar = nullptr;

    BluetoothReconnectTask bluetoothReconnectTask;

    void startScan();
    bool connect();
    void disconnect();
};

#endif //BLUETOOTH_H
