#pragma once

#include <cstdint>

#include "../Projdefs.hpp"

#include <queue>

namespace PINYMOTOR {

// 类型擦除 管理异构CRTP
class IMotor {
protected:
    Model_s model_;
    Data_s data_;
    float txFreq_;

    std::queue<MotorErrorCode_e> errQueue_;
public:
    virtual ~IMotor() = default;
    virtual MotorTypedef_e parse(uint8_t *_rxBuffer) = 0;
    virtual MotorTypedef_e ctrl() = 0;

    virtual MotorTypedef_e cmd(MotorCmdType_e _cmd, float _cmdData) = 0;

    virtual uint16_t uid() = 0;

    Data_s &data() { return data_; }
    // 出轴减速比
    float RR(){ return model_.reductionRatio; }
    // 角度编码量程
    float span(){return static_cast<float>(model_.measureMax - model_.measureMin);}
};
}