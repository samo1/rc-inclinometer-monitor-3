#include "bluetooth.h"
#include "debug.h"
#include "display_header.h"

#define BLUETOOTH_RECONNECT_DELAY_MS 5000

bool scanInProgress = false;

void scanCompleteCallback(BLEScanResults scanResults) {
    DEBUG_PRINTLN("Scan complete");
    scanInProgress = false;
}

void Bluetooth::initialize() {
    BLEDevice::init("");
    startScan();
}

void Bluetooth::loop() {
    if (doConnect) {
        doConnect = false;
        if (!connect()) {
            disconnect();
        }
    } else {
        // DEBUG_PRINT("BLUETOOTH connected="); DEBUG_PRINT(connected);
        // DEBUG_PRINT(" connectInProgress="); DEBUG_PRINT(connectInProgress);
        // DEBUG_PRINT(" doReconnect="); DEBUG_PRINT(doReconnect);
        // DEBUG_PRINT(" scanInProgress="); DEBUG_PRINT(scanInProgress);
        // DEBUG_PRINTLN("");

        if (!connected && !connectInProgress && !doReconnect && !scanInProgress) {
            doReconnect = true;
            bluetoothReconnectTask.restartDelayed(BLUETOOTH_RECONNECT_DELAY_MS);
        }
    }
}

void Bluetooth::onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(infoServiceUUID)) {
        BLEDevice::getScan()->stop();
        DEBUG_PRINTLN("Device found, scan stopped");
        scanInProgress = false;
        serverDevice = new BLEAdvertisedDevice(advertisedDevice);
        doConnect = true;
    }
}

void Bluetooth::onConnect(BLEClient *pClient) {
    connectInProgress = false;
    connected = true;
    DisplayHeader::printText("Connected");
    DisplayHeader::drawBluetoothConnectedImage();
}

void Bluetooth::onDisconnect(BLEClient *pClient) {
    connectInProgress = false;
    connected = false;
    DisplayHeader::printText("Disconnected");
    DisplayHeader::drawBluetoothDisabledImage();
}

void Bluetooth::startScan() {
    DisplayHeader::printText("Connecting...");
    DisplayHeader::drawBluetoothSearchingImage();

    scanInProgress = true;

    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(this);
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(5, &scanCompleteCallback, false);
}

bool Bluetooth::connect() {
    connectInProgress = true;

    delete client;
    client = BLEDevice::createClient();
    client->setClientCallbacks(this);

    DEBUG_PRINTLN("Connecting to remote BLE device");
    DEBUG_PRINTLN(serverDevice->getAddress().toString().c_str());

    bool ok = client->connect(serverDevice);
    client->setMTU(517);

    if (!ok) {
        DEBUG_PRINTLN("Connection failed");
        return false;
    }

    DEBUG_PRINTLN("Connected, getting the service");

    BLERemoteService* remoteService = client->getService(infoServiceUUID);
    if (remoteService == nullptr) {
        DEBUG_PRINTLN("Info service not found");
        return false;
    }

    pitchRollChar = remoteService->getCharacteristic(pitchRollCharUUID);
    if (pitchRollChar == nullptr) {
        DEBUG_PRINTLN("Pitch-Roll characteristic not found");
        return false;
    }

    // pitchRollChar->registerForNotify()

    return true;
}

void Bluetooth::disconnect() {
    client->disconnect();
}

bool BluetoothReconnectTask::Callback() {
    bluetooth.doReconnect = false;
    if (!bluetooth.connected && !bluetooth.connectInProgress && !scanInProgress) {
        bluetooth.startScan();
    }
    return true;
}
