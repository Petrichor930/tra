#pragma once

#include "../HAL/CAN/CANGroup.hpp"

#include <vector>

namespace PINYMOTOR {
class MotorManager {
public:
    MotorManager();
    void registerGroup(CanGroupBase *_group);
    void run();

private:
    std::vector<CanGroupBase *> groups_;
    QueueHandle_t canRxQueue_;

    void processRxFrames_();
    void updateAllGroups_(uint32_t _currentTime);
};
};
