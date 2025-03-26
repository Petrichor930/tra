#include "MotorManager.hpp"

#include "../../../../System/RTOS/FreeRTOS/include/task.h"

using namespace PINYMOTOR;
MotorManager::MotorManager()
{
    canRxQueue_ = xQueueCreate(10, sizeof(CanFrame_s));
    if (halCanRegRxQueue) {
        halCanRegRxQueue(canRxQueue_);
    }
}

void MotorManager::registerGroup(CanGroupBase *_group) { groups_.push_back(_group); }

void MotorManager::run()
{
    uint32_t lastWake = xTaskGetTickCount();
    while (true) {
        processRxFrames_();
        updateAllGroups_(xTaskGetTickCount());
        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(1));
    }
}

void MotorManager::processRxFrames_()
{
    CanFrame_s frame;
    while (xQueueReceive(canRxQueue_, &frame, 0) == pdTRUE) {
        for (auto *group : groups_) {
            group->parseFrame(frame);
        }
    }
}

void MotorManager::updateAllGroups_(uint32_t _currentTime)
{
    for (auto *group : groups_) {
        group->update(_currentTime);
    }
}
