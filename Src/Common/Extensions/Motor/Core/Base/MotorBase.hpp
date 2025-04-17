#pragma once

#include "IMotor.hpp"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include <unordered_map>
#include <cstring>
#include <functional>

namespace PINYMOTOR {

// 模板类，用于定义一拖一电机的基类
template <typename Derived> class MotorBase : public IMotor {
private:
    inline Derived &derived() { return static_cast<Derived &>(*this); }
    inline const Derived &derived() const { return static_cast<const Derived &>(*this); }
protected:

    uint32_t *pComHandle_;
    ComType_e comType_;
    WorkMode_e workMode_;
    GlobalState_e globalState_;
    uint8_t offsetId_;

    // 用户自定义回调函数
    std::function<void(uint8_t *_rxBuffer)> userRecvCallback_;


    bool isQuad_ = false; // default is not quad encoder

    uint32_t lastSendTick = 0; // ms

public:
    // 构造函数
    MotorBase(const char _name[16], InitConfig_s _config);

    // 解析接收到的数据
    inline MotorTypeDef_e parse(uint8_t *_rxBuffer) override final  { return derived()._parse_(_rxBuffer); }

    // 控制电机
    inline MotorTypeDef_e ctrl() override final { return derived()._ctrl_(); }

    // 编辑指令
    inline MotorTypeDef_e cmd(MotorCmdType_e _cmd, float _cmdData) override final
    {
        return derived()._cmd_(_cmd, _cmdData);
    }
    inline MotorTypeDef_e cmd(MotorCmdType_e _cmd) override final
    {
        return derived()._cmd_(_cmd);
    }

    // 注册用户自定义接收回调函数
    inline void
    regUserRecvCallback(std::function<void(uint8_t *_rxBuf)> _callback)
    {
        userRecvCallback_ = std::move(_callback);
    }

    // 发送ID
    inline uint16_t canId() { return derived().canId(); }

    // 反馈ID 
    inline uint16_t masterId() { return derived().masterId(); }

    // 唯一标识符
    inline uint16_t uid() override final { return derived()._uid_();}

    // 检查是否需要发送数据
    inline bool checkSend() const
    {
        return (xTaskGetTickCount() - lastSendTick) >=
               pdMS_TO_TICKS(1000.f / this->txFreq_);
    }
};

struct QuadMotorGroup_s {
    IMotor *motor[4];
    QuadMotorGroup_s() { for(int i = 0; i < 4; i++) motor[i] = nullptr; }
};
// 模板类，用于定义一拖四电机的基类
template <typename Derived> class QuadMotorBase : public MotorBase<Derived> {
protected:
    // 使用基类的成员变量和成员函数
    using Base = MotorBase<Derived>;
    using Base::model_;

    // vector < pair(pComHandle_, <canId, 4 motors>) >
    static std::vector<std::pair<uint32_t *, std::unordered_map<uint16_t, QuadMotorGroup_s *> > >
            motorMap_;

public:
    // 构造函数，初始化基类，并将isQuad_设置为true
    inline QuadMotorBase(const char _name[16], InitConfig_s _config);
    
    // 获取电机的组ID
    inline uint32_t getGroupId() const override { return model_.txBaseId_; }

    // 获取电机在组中的位置
    inline uint8_t getPosInGroup() const
    {
        return this->offsetId_ % 4; // 0,1,2,3
    }
};
}
