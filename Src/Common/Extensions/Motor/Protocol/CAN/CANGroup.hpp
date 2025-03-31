#pragma once

#include <cstdint>

#include "FreeRTOS.h"

#include "queue.h"

namespace PINYMOTOR {

#pragma pack(push, 1)
struct CanFrame_s {
    uint32_t id;
    uint8_t data[8];
    size_t size;
};
#pragma pack(pop)

using HalCanSendFunc = void (*)(uint32_t _canId, const uint8_t *_data, size_t _size);
using HalCanRegRxQueueFunc = void (*)(QueueHandle_t _queue);
extern HalCanSendFunc halCanSend;
extern HalCanRegRxQueueFunc halCanRegRxQueue;

class CanGroupBase {
public:
    virtual void update(uint32_t _currentTime) = 0;
    virtual void parseFrame(const CanFrame_s &_frame) = 0;
    virtual void notifyPending() = 0;
    virtual ~CanGroupBase() = default;
};
}
