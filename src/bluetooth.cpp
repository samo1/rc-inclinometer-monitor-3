#include "bluetooth.h"
#include "debug.h"
#include "display_header.h"

void Bluetooth::initialize() {
    DisplayHeader::printText("Connecting...");
    DisplayHeader::drawBluetoothSearchingImage();

    BLEDevice::init("");

    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(this);
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(5, false);
}

void Bluetooth::loop() {
    if (doConnect) {
        doConnect = false;
        if (!connect()) {
            disconnect();
        }
    }
}

void Bluetooth::onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(infoServiceUUID)) {
        BLEDevice::getScan()->stop();
        serverDevice = new BLEAdvertisedDevice(advertisedDevice);
        doConnect = true;
    }
}

void Bluetooth::onConnect(BLEClient *pClient) {
    connected = true;
    DisplayHeader::printText("Connected");
    DisplayHeader::drawBluetoothConnectedImage();
}

void Bluetooth::onDisconnect(BLEClient *pClient) {
    connected = false;
    DisplayHeader::printText("Disconnected");
    DisplayHeader::drawBluetoothDisabledImage();
}

bool Bluetooth::connect() {
    delete client;
    client = BLEDevice::createClient();
    client->setClientCallbacks(this);

    DEBUG_PRINTLN("Connecting to remote BLE device");
    DEBUG_PRINTLN(serverDevice->getAddress().toString().c_str());

    bool c = client->connect(serverDevice);
    client->setMTU(517);

    if (!c) {
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
