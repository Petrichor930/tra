#pragma once

#include "IMotor.hpp"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include <unordered_map>

namespace PINYMOTOR {

// 模板类，用于定义一拖一电机的基类
template <typename Derived> class MotorBase : public IMotor {
protected:

    uint32_t *pComHandle_;
    ComType_e comType_;
    WorkMode_e workMode_;
    GlobalState_e globalState_;
    uint8_t offsetId_;

    bool isQuad_ = false; // default is not quad encoder

    uint32_t lastSendTick = 0; // ms

    // 定义发送互斥信号量
    static SemaphoreHandle_t sendMutex_;

    // 定义派生类对象
    Derived &derived() { return static_cast<Derived &>(*this); }
    const Derived &derived() const { return static_cast<const Derived &>(*this); }

public:
    // 构造函数
    MotorBase(InitConfig_s _config)
            : pComHandle_(_config.pComHandle)
            , comType_(_config.comType)
            , workMode_(_config.workMode)
            , globalState_(GlobalState_e::UNREGISTER)  // 需明确初始化
            , offsetId_(_config.offsetId)
    {
        this->txFreq_ = _config.txFreq;
        // Base class constuctor
    }

    // 解析接收到的数据
    MotorTypedef_e parse(uint8_t *_rxBuffer) override final  { return derived()._parse_(_rxBuffer); }

    // 控制电机
    MotorTypedef_e ctrl() override final { return derived()._ctrl_(); }

    // 编辑指令
    MotorTypedef_e cmd(MotorCmdType_e _cmd, float _cmdData) override final
    {
        return derived()._cmd_(_cmd, _cmdData);
    }

    // 发送ID
    uint16_t canId() { return derived().canId(); }

    // 反馈ID
    uint16_t masterId() { return derived().masterId(); }

    // 唯一标识符
    uint16_t uid() override final { return derived()._uid_();}

    // 检查是否需要发送数据
    bool checkSend(uint32_t _interval) const
    {
        return (xTaskGetTickCount() - lastSendTick) >= pdMS_TO_TICKS(_interval);
    }

};
template <typename Derived>
SemaphoreHandle_t MotorBase<Derived>::sendMutex_ = xSemaphoreCreateMutex();


struct QuadMotorGroup_s {
    IMotor *motor[4];
};
// 模板类，用于定义一拖四电机的基类
template <typename Derived> class QuadMotorBase : public MotorBase<Derived> {
protected:
    // 使用基类的成员变量和成员函数
    using Base = MotorBase<Derived>;
    using Base::model_;

    // 二重键值对 pComHandle_ -> CanId -> QuadMotorGroup
    static std::unordered_map<uint32_t *, std::unordered_map<uint16_t, QuadMotorGroup_s *> >
            motorMap_;

public:
    // 构造函数，初始化基类，并将isQuad_设置为true
    QuadMotorBase(InitConfig_s _config) : Base(_config) { this->isQuad_ = true; }
    // 获取电机的组ID
    uint32_t getGroupId() const override { return model_.txBaseId_; }
};

}
