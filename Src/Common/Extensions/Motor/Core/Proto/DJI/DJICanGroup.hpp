#pragma once

#include "../../Base/MotorBase.hpp"

namespace PINYMOTOR {
template <typename Protocol> class DJICanGroup : public CanGroupBase {
private:
    Motor<Protocol> *motors_[Protocol::MOTORS_PER_FRAME];
    uint32_t lastSendTime_;
    uint32_t sendInterval_;
    bool pending_;
    typename Protocol::Mode_e currentMode_;

    SemaphoreHandle_t groupMutex_;

public:
    DJICanGroup(uint32_t interval) : lastSendTime_(0), sendInterval_(interval), pending_(false) {}

    void notifyPending() override final
    {
        xSemaphoreTake(groupMutex_, portMAX_DELAY);
        pending_ = true;
        xSemaphoreGive(groupMutex_);
    }

    void addMotor(size_t idx, Motor<Protocol> *_motor)
    {
        if (idx < Protocol::MOTORS_PER_FRAME) {
            motors_[idx] = _motor;
            _motor->group = this;
        }
    }

    void update(uint32_t _currentTime) override final
    {
        if ((_currentTime - lastSendTime_) >= sendInterval_ && pending_) {
            xSemaphoreTake(groupMutex_, portMAX_DELAY);
            uint8_t buffer[8] = { 0 };
            // 生成命令数据...
            halCanSend(Protocol::getCanId(currentMode_), buffer, sizeof(buffer));
            xSemaphoreGive(groupMutex_);
            lastSendTime_ = _currentTime;
            pending_ = false;
        }
    }

    void parseFrame(const CanFrame_s &_frame) override final
    {
        // 解析反馈数据...
    }
};
};
