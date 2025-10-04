#include "RcMsgHandler.hpp"
#include "Arm.hpp"

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


void RcMsgHandler::updateRocker(float &_target, float _channel)
{
    float delta = (_channel * T_ACC_CNT / 660.0f) - _target;
    _target += std::fmax(-S_CURVE_ACC, std::fmin(S_CURVE_ACC, delta));
}

void RcMsgHandler::init(MsgBus_s *_bus, EventGroupHandle_t _event)
{
    msgBus_ = _bus;
    this->event = _event;
    rc = std::make_unique<RC::Rc>(&RC_UART);
    rc->init(_event);
}


void RcMsgHandler::handle()
{
    rc->parseData();
    RC::RcRawMsg_t rcData = rc->getData();

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
    } else {
        cmsg.state = CHASSIS::FSMState_e::STOP;
        amsg.state = ARM::FSMState_e::STOP;
        cmsg.vx = 0.f;
        cmsg.vy = 0.f;
        cmsg.yaw = 0.f;
    }

    memcpy(&rcMsgPrev_, &rcMsg_, sizeof(rcMsg_));

    notify(&cmsg, msgBus_->chassisQueue);
    notify(&amsg, msgBus_->armQueue);
}


void RcMsgHandler::notify(Msg *_msg, QueueHandle_t _queue)
{
    xQueueSend(_queue, _msg, 0);
}
