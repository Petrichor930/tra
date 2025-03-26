#include "MotorBase.hpp"

#include "../../../../../System/RTOS/FreeRTOS/include/task.h"

using namespace PINYMOTOR;
template <typename Protocol>
Motor<Protocol>::Motor(uint8_t _id, float _gearRatio)
        : gearRatio(_gearRatio), id(_id), group(nullptr)
{
    mutex_ = xSemaphoreCreateMutex();
}

template <typename Protocol> void Motor<Protocol>::setTorque(float _torque)
{
    xSemaphoreTake(mutex_, portMAX_DELAY);
    mode = Protocol::Mode_e::TORQUE;
    commandData.torque = _torque;
    xSemaphoreGive(mutex_);
    if (group)
        group->notifyPending();
}
