#include "Mecanum.hpp"
#include "sdkconfig.h"
#include "Bsp_can.hpp"
#include "PidBasic.hpp"
#include <numbers>
#include "Cmd.hpp"

#include "M3508.hpp"

#include "QuadricycleController.hpp"

#include "./State/StopState.hpp"
#include "./State/RunState.hpp"

extern canHandle HCAN1;

extern Cmd *cmd;

using namespace CHASSIS;
using namespace PINYMOTOR;

constexpr float F_PI = std::numbers::pi_v<float>;

IncrementalPid motorPids[4] = { IncrementalPid(0.90f, 0.002f, 0.f, 4.0f, 0.0f),
                                IncrementalPid(0.90f, 0.002f, 0.f, 4.0f, 0.0f),
                                IncrementalPid(0.90f, 0.002f, 0.f, 4.0f, 0.0f),
                                IncrementalPid(0.90f, 0.002f, 0.f, 4.0f,
                                               0.0f) };

// IncrementalPid motorPids[4] = { IncrementalPid(0.91f, 0.002f, 0.f, 4.0f, 0.0f),
//                                 IncrementalPid(0.91f, 0.002f, 0.f, 4.0f, 0.0f),
//                                 IncrementalPid(0.88f, 0.002f, 0.f, 4.0f, 0.0f),
//                                 IncrementalPid(0.88f, 0.002f, 0.f, 4.0f,
//                                                0.0f) };

Mecanum::Mecanum()
{
    for (uint8_t i = 1; i <= 4; i++) {
        InitConfig_s m3508Config = { .pComHandle = reinterpret_cast<uint32_t *>(
                                             &HCAN1),
                                     .comType = ComType_e::CAN,
                                     .workMode = WorkMode_e::QUAD_CURR,
                                     .offsetId = i,
                                     .txFreq = 1000.0f,
                                     .posPID = nullptr,
                                     .velPID = &motorPids[i - 1],
                                     .torqPID = nullptr };
        motors_._[i - 1] = new DJIMOTOR::M3508("M3508", m3508Config);
    }

    this->stateFactory.addState(FSMState_e::RUN,
                                std::make_unique<RunState>(this));
    this->stateFactory.addState(FSMState_e::STOP,
                                std::make_unique<StopState>(this));
    this->stateFactory.init(this->stateFactory.getNextState(FSMState_e::STOP));
}

void Mecanum::stop()
{
    for (auto &i : motors_._) {
        i->cmd(PINYMOTOR::MotorCmdType_e::OFF);
    }
}

void Mecanum::enter()
{
    for (auto &i : motors_._) {
        i->cmd(PINYMOTOR::MotorCmdType_e::ON);
    }
}

void Mecanum::update()
{
    xQueueReceive(cmd->getMsgBus()->chassisQueue, &msg, 0);

    for (uint8_t i = 0; i < 4; i++) {
        wSpeed_._[i] = iir3_.process(motors_._[i]->data().spdRpm);
    }
    this->curSpeed_ = forward(wSpeed_);

    this->stateFactory.update();
}


Speed_u Mecanum::forward(const WheelsSpeed_u &_wSpeed)
{
    Speed_u speed;
    speed.vx = W_CIRCUMFERENCE *
               (_wSpeed.M_LF - _wSpeed.M_RF + _wSpeed.M_LB - _wSpeed.M_RB) /
               4.f / 60.f;
    speed.vy = W_CIRCUMFERENCE *
               (BACK_R * -_wSpeed.M_RF - BACK_R * _wSpeed.M_LF +
                FRONT_R * _wSpeed.M_LB + FRONT_R * _wSpeed.M_RB) /
               (2.f * (BACK_R + FRONT_R)) / 60.f;
    speed.wz = (W_DIAMETER / 2.f) *
               (_wSpeed.M_RF + _wSpeed.M_LF + _wSpeed.M_LB + _wSpeed.M_RB) /
               (2.f * (BACK_R + FRONT_R)) * (2.f * F_PI) / 60.f;
    return speed;
}


WheelsSpeed_u Mecanum::reverse(const Speed_u &_speed)
{
    WheelsSpeed_u refWheels;
    refWheels.M_RF = (60.f / W_CIRCUMFERENCE) * (-_speed.vx - _speed.vy) +
                     FRONT_R / (W_DIAMETER / 2.f) * _speed.wz * 30.f / F_PI;
    refWheels.M_LF = (60.f / W_CIRCUMFERENCE) * (_speed.vx - _speed.vy) +
                     FRONT_R / (W_DIAMETER / 2.f) * _speed.wz * 30.f / F_PI;
    refWheels.M_LB = (60.f / W_CIRCUMFERENCE) * (_speed.vx + _speed.vy) +
                     BACK_R / (W_DIAMETER / 2.f) * _speed.wz * 30.f / F_PI;
    refWheels.M_RB = (60.f / W_CIRCUMFERENCE) * (-_speed.vx + _speed.vy) +
                     BACK_R / (W_DIAMETER / 2.f) * _speed.wz * 30.f / F_PI;
    return refWheels;
}

void Mecanum::ctrl(const Speed_u &_refSpeed)
{
    float cosYaw = arm_cos_f32(deltaYaw_);
    float sinYaw = arm_sin_f32(deltaYaw_);
    Speed_u refSpeed = {
        .vx = (_refSpeed.vx * cosYaw) - (_refSpeed.vy * sinYaw),
        .vy = (_refSpeed.vy * cosYaw) + (_refSpeed.vx * sinYaw),
        .wz = _refSpeed.wz
    };
    WheelsSpeed_u refWSpeed = reverse(refSpeed);

    for (uint8_t i = 0; i < 4; i++) {
        motors_._[i]->cmdVel(rpm2radps(refWSpeed._[i]));
    }
}
