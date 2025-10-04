#pragma once

#include "FSMState.hpp"
#include "StmLog.hpp"
#include "MsgImpl.hpp"

namespace CHASSIS {
enum FSMMode_e : uint8_t;
}

class Chassis {
public:
    static constexpr float MAX_VX_SPEED = 18.f;
    static constexpr float MAX_VY_SPEED = 18.f;
    static constexpr float MAX_WZ_SPEED = 30.f;
    static constexpr float MOUSE_CAL = 200.f;

    void update(void *_param);

    StateFactory<CHASSIS::FSMState_e> stateFactory;

    LOG::Logger &log = LOG::Logger::instance();

    ChassisMsg_s msg = {};

    float deltaYaw_ = 0.f;
};
