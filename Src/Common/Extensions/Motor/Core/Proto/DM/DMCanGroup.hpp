#pragma once

#include "../../Base/MotorBase.hpp"

namespace PINYMOTOR {
template <typename Protocol> class DMGroup : public CanGroupBase {
    Motor<Protocol> *motor_;
    uint32_t lastSendTime_;
    uint32_t sendInterval_;
    bool pending_;

public:
    DMGroup(uint32_t _interval) : lastSendTime_(0), sendInterval_(_interval), pending_(false) {}

    void notifyPending() override final
    {
        xSemaphoreTake(motor_->mutex, portMAX_DELAY);
        pending_ = true;
        xSemaphoreGive(motor_->mutex);
    }

    void setMotor(Motor<Protocol> *_m)
    {
        motor_ = _m;
        _m->group = this;
    }

    void update(uint32_t _currentTime) override final
    {
        if (motor_ && (_currentTime - lastSendTime_) >= sendInterval_ && pending_) {
            xSemaphoreTake(motor_->mutex, portMAX_DELAY);
            uint8_t buffer[8] = { 0 };
            // 生成命令数据...
            halCanSend(Protocol::getCanId(motor_->mode), buffer, sizeof(buffer));
            xSemaphoreGive(motor_->mutex);
            lastSendTime_ = _currentTime;
            pending_ = false;
        }
    }

    void parseFrame(const CanFrame_s &_frame) override final
    {
        if (motor_)
            Protocol::parseFeedback(motor_, _frame.data);
    }
};
};