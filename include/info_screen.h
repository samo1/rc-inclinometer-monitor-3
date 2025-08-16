#pragma once

#include "battery_task.h"
#include "bluetooth.h"
#include "display_main_area.h"
#include "info_screen_selection.h"
#include "state.h"
#include "task_scheduler.h"

class InfoScreen : public Task {
public:
    InfoScreen(StateManager &stateManager, DisplayMainArea &displayMainArea, BatteryTask &batteryTask,
               Bluetooth &bluetooth)
            : Task(200, TASK_FOREVER, &scheduler, false),
              stateManager(stateManager),
              displayMainArea(displayMainArea),
              batteryTask(batteryTask),
              bluetooth(bluetooth) {
        setSchedulingOption(TASK_INTERVAL);
    }

    bool Callback() override;
    void handleButtonClick();
    void handleButtonLongPress();

private:
    StateManager &stateManager;
    DisplayMainArea &displayMainArea;
    BatteryTask &batteryTask;
    Bluetooth &bluetooth;
    InfoScreenSelection selection = InfoScreenSelection::NONE;
    InfoScreenSelection previousSelection = InfoScreenSelection::NONE;
    boolean displayInitialized = false;
    uint16_t previousBatVoltRaw = 0;
    uint16_t previousBatteryMinutes = 0;
    unsigned long previousTickNr = 0;
    double previousDistance = 0.0;
    double mmDistancePerRevolution = 0.0;
    double previousMmDistancePerRevolution = 0.0;

    void resetDistance();
    void setWheelSize(double wheelSize);
};
