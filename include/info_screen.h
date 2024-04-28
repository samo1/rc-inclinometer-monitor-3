#pragma once

#include "battery_task.h"
#include "bluetooth.h"
#include "display_main_area.h"
#include "state.h"
#include "task_scheduler.h"

class InfoScreen : public Task {
public:
    InfoScreen(StateManager& stateManager, DisplayMainArea& displayMainArea, BatteryTask& batteryTask)
            : Task(200, TASK_FOREVER, &scheduler, false),
              stateManager(stateManager),
              displayMainArea(displayMainArea),
              batteryTask(batteryTask) {
        setSchedulingOption(TASK_INTERVAL);
    }

    bool Callback() override;

private:
    StateManager& stateManager;
    DisplayMainArea& displayMainArea;
    BatteryTask& batteryTask;
    boolean displayInitialized = false;
    uint16_t previousBatVoltRaw = 0;
    unsigned long previousSeconds = 0;
    unsigned long previousTickNr = 0;
};
