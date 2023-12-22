#include "bluetooth.h"
#include "debug.h"

#define BLUETOOTH_RECONNECT_DELAY_MS 5000

bool scanInProgress = false;

double pitch = 0.0;
double roll = 0.0;

bool winchEnabled = false;
char winchMovement = 'S';
bool frontDigEnabled = false;

double currentSpeedValue = 0.0;
double currectDistanceValue = 0.0;
unsigned long tickNr = 0;

static void scanCompleteCallback(BLEScanResults scanResults) {
    DEBUG_PRINTLN("Scan complete");
    scanInProgress = false;
}

static void pitchRollUpdate(std::string& stringData) {
    auto pos = stringData.find(':');
    if (pos != std::string::npos) {
        std::string pitchString = stringData.substr(0, pos);
        std::string rollString = stringData.substr(pos + 1);
        try {
            pitch = std::stod(pitchString);
            roll = std::stod(rollString);
        }
        catch (const std::invalid_argument &e) {}
        catch (const std::out_of_range &e) {}
    }
}

static void pitchRollCharNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic,
                                        uint8_t* pData, size_t length, bool isNotify) {
    std::string stringData(pData, pData + length);
    DEBUG_PRINT("Received pitch roll callback ");
    DEBUG_PRINTLN(stringData.c_str());
    pitchRollUpdate(stringData);
}

static void winchInfoUpdate(std::string& stringData) {
    if (stringData.length() >= 3) {
        winchEnabled = (stringData[0] == 'E');
        winchMovement = stringData[1];
        frontDigEnabled = (stringData[2] == 'E');
    }
}

static void winchInfoCharNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic,
                                        uint8_t* pData, size_t length, bool isNotify) {
    std::string stringData(pData, pData + length);
    DEBUG_PRINT("Received winch info callback ");
    DEBUG_PRINTLN(stringData.c_str());
    winchInfoUpdate(stringData);
}

static void speedUpdate(std::string& stringData) {
    auto pos1 = stringData.find(';');
    auto pos2 = stringData.find(';', pos1 + 1);
    if (pos1 != std::string::npos && pos2 != std::string::npos) {
        std::string speedString = stringData.substr(0, pos1);
        std::string tickNrString = stringData.substr(pos1 + 1, pos2);
        std::string distanceString = stringData.substr(pos2 + 1);
        try {
            currentSpeedValue = std::stod(speedString);
            currectDistanceValue = std::stod(distanceString);
            tickNr = std::stoul(tickNrString);
        }
        catch (const std::invalid_argument &e) {}
        catch (const std::out_of_range &e) {}
    }
}

static void speedCharNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic,
                                    uint8_t* pData, size_t length, bool isNotify) {
    std::string stringData(pData, pData + length);
    DEBUG_PRINT("Received speed callback ");
    DEBUG_PRINTLN(stringData.c_str());
    speedUpdate(stringData);
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
    stateManager.setState(State::connected);
}

void Bluetooth::onDisconnect(BLEClient *pClient) {
    connectInProgress = false;
    connected = false;
    stateManager.setState(State::disconnected);
}

void Bluetooth::startScan() {
    stateManager.setState(State::connecting);

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
    if (pitchRollChar->canNotify()) {
        pitchRollChar->registerForNotify(pitchRollCharNotifyCallback);
    }
    if (pitchRollChar->canRead()) {
        std::string pitchRollValue = pitchRollChar->readValue();
        pitchRollUpdate(pitchRollValue);
    }

    winchInfoChar = remoteService->getCharacteristic(winchInfoCharUUID);
    if (winchInfoChar == nullptr) {
        DEBUG_PRINTLN("Winch-Info characteristic not found");
        return false;
    }
    if (winchInfoChar->canNotify()) {
        winchInfoChar->registerForNotify(winchInfoCharNotifyCallback);
    }
    if (winchInfoChar->canRead()) {
        std::string winchInfoValue = winchInfoChar->readValue();
        winchInfoUpdate(winchInfoValue);
    }

    winchControlChar = remoteService->getCharacteristic(winchControlCharUUID);

    speedChar = remoteService->getCharacteristic(speedCharUUID);
    if (speedChar == nullptr) {
        DEBUG_PRINTLN("Speed characteristic not found");
        return false;
    }
    if (speedChar->canNotify()) {
        speedChar->registerForNotify(speedCharNotifyCallback);
    }
    if (speedChar->canRead()) {
        std::string speedValue = speedChar->readValue();
        speedUpdate(speedValue);
    }

    return true;
}

void Bluetooth::disconnect() {
    client->disconnect();
}

double Bluetooth::getPitch() {
    return pitch;
}

double Bluetooth::getRoll() {
    return roll;
}

bool Bluetooth::getWinchEnabled() {
    return winchEnabled;
}

char Bluetooth::getWinchMovement() {
    return winchMovement;
}

bool Bluetooth::getFrontDigEnabled() {
    return frontDigEnabled;
}

double Bluetooth::getSpeed() {
    return currentSpeedValue;
}

double Bluetooth::getDistance() {
    return currectDistanceValue;
}

unsigned long Bluetooth::getTickNr() {
    return tickNr;
}

void Bluetooth::sendCommand(std::string& cmd) {
    if (winchControlChar->canWrite()) {
        winchControlChar->writeValue(cmd, true);
    }
}

bool BluetoothReconnectTask::Callback() {
    bluetooth.doReconnect = false;
    if (!bluetooth.connected && !bluetooth.connectInProgress && !scanInProgress) {
        bluetooth.startScan();
    }
    return true;
}
