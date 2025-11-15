#include "RcMsgHandler.hpp"
#include "Arm.hpp"


#include "Pump.hpp"
#include "RcMsg.hpp"
#include <algorithm>
#include "sdkconfig.h"

#ifdef CHASSIS_TYPE
#include CHASSIS_FILE
#endif
#ifdef GIMBAL_TYPE
#include GIMBAL_FILE
#endif

#include "Smooth.hpp"

#include <cmath>
#include <cstring>

RC::RcRawMsg_t arcData;
void RcMsgHandler::updateRocker(float &_target, float _channel)
{
    float delta = (_channel * T_ACC_CNT / 660.0f) - _target;
    _target += std::fmax(-S_CURVE_ACC, std::fmin(S_CURVE_ACC, delta));
}
void RcMsgHandler::kupdateRocker(float &_target, float _channel)
{
    float delta = (_channel * T_ACC_CNT) - _target;
    _target += std::fmax(-S_CURVE_ACC, std::fmin(S_CURVE_ACC, delta));
}

void RcMsgHandler::init(MsgBus_s *_bus, EventGroupHandle_t _event)
{
    msgBus_ = _bus;
    this->event = _event;
    rc = std::make_unique<RC::Rc>(&RC_UART);
    rc->init(_event);
}

uint32_t RcMsgHandler::isKeyPressed(uint16_t _key)
{
    RC::RcRawMsg_t rcData = rc->getData();
    return ((rcData.keyboard.keyCode & _key) == _key);
}

void RcMsgHandler::handle()
{
    rc->parseData();
    RC::RcRawMsg_t rcData = rc->getData();
    arcData = rcData;
    updateRocker(rcMsg_.rx, (float)rcData.rc.ch0);
    updateRocker(rcMsg_.ry, (float)rcData.rc.ch1);
    updateRocker(rcMsg_.lx, (float)rcData.rc.ch2);
    updateRocker(rcMsg_.ly, (float)rcData.rc.ch3);

    rcMsg_.rSwitch = rcData.rc.switchRight;
    rcMsg_.lSwitch = rcData.rc.switchLeft;

    rcMsg_.xMove = rcData.mouse.x;
    rcMsg_.yMove = rcData.mouse.y;
    rcMsg_.zRoller = rcData.mouse.z;

    if (rcData.rc.switchRight == RC_SW_DOWN) {
        cmsg.state = CHASSIS::FSMState_e::STOP;
        amsg.state = ARM::FSMState_e::STOP;
        pump_.apply(PUMP::ALL_OFF);
        cmsg.vx = 0.f;
        cmsg.vy = 0.f;
        cmsg.yaw = 0.f;
    } else if (rcData.rc.switchRight == RC_SW_MID) {
        amsg.state = ARM::FSMState_e::NORMAL;
        if (rcData.rc.switchLeft == RC_SW_DOWN) {
            cmsg.state = CHASSIS::FSMState_e::RUN;
            cmsg.vx = sCurve(Chassis::MAX_VX_SPEED, rcMsg_.ry);  // Scale to m/s
            cmsg.vy = sCurve(Chassis::MAX_VY_SPEED, -rcMsg_.rx); // Scale to m/s
            cmsg.yaw = sCurve(Chassis::MAX_WZ_SPEED, rcMsg_.lx); // Scale to m/s
        } else if (rcData.rc.switchLeft == RC_SW_MID) {
            amsg.target.j[0] = rcMsg_.rx / 314 / 200;
            amsg.target.j[1] = rcMsg_.ry / 314 / 100;
            amsg.target.j[2] = rcMsg_.ly / 314 / 150;
            amsg.target.j[3] = rcMsg_.lx / 314 / 50;
        } else if (rcData.rc.switchLeft == RC_SW_UP) {
            amsg.target.j[4] = -rcMsg_.lx / 314 / 100;
            amsg.target.j[5] = rcMsg_.ly / 314 / 50;
            amsg.target.j[6] = rcMsg_.rx / 314 / 50;
            if (rcData.rc.ch1 == -660) {
                amsg.state = ARM::FSMState_e::PLAN;
            }
        }
    } else if (rcData.rc.switchRight == RC_SW_UP) {
        // pump_.apply(PUMP::ALL_ON);
        cmsg.state = CHASSIS::FSMState_e::RUN;

        if (isKeyPressed(RC::SHIFT))
            accK_ = 1.5f;
        else
            accK_ = 1.f;

        if (isKeyPressed(RC::W)) {
            kupdateRocker(rcMsg_.ry, 1); //加速向前，ry 增加并趋近于最大值。
        } else if (isKeyPressed(RC::S)) {
            updateRocker(rcMsg_.ry, -1); //加速向后，ry 减少并趋近于最小值。
        } else {
            kupdateRocker(rcMsg_.ry, 0); //自然减速回中，ry 向 0 值靠近。
        }

        if (isKeyPressed(RC::A)) {
            kupdateRocker(rcMsg_.rx, 1);
        } else if (isKeyPressed(RC::D)) {
            kupdateRocker(rcMsg_.rx, -1);
        } else {
            kupdateRocker(rcMsg_.rx, 0);
        }

        if (rcData.rc.switchLeft == 1)
            kupdateRocker(rcMsg_.lx, -1);
        else
            kupdateRocker(rcMsg_.lx, 0);

        if (rcData.rc.switchRight == 1)
            kupdateRocker(rcMsg_.lx, 1);
        else
            kupdateRocker(rcMsg_.lx, 0);


        // rcMsg_.lx = ((float)rcData->mouse.y * (GAIN_MOUSE_X));
        // rcMsg_.ly = -((float)rcData->mouse.x * (GAIN_MOUSE_Y));

        cmsg.vx = sCurve(accK_ * Chassis::MAX_VX_SPEED,
                         rcMsg_.ry); // Scale to m/s
        cmsg.vy = sCurve(accK_ * Chassis::MAX_VY_SPEED,
                         -rcMsg_.rx); // Scale to m/s
        cmsg.yaw = sCurve(accK_ * Chassis::MAX_WZ_SPEED,
                          rcMsg_.lx); // Scale to m/s
    }

    memcpy(&rcMsgPrev_, &rcMsg_, sizeof(rcMsg_));

    notify(&cmsg, msgBus_->chassisQueue);
    notify(&amsg, msgBus_->armQueue);
}


void RcMsgHandler::notify(Msg *_msg, QueueHandle_t _queue)
{
    xQueueSend(_queue, _msg, 0);
}
