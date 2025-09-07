#pragma once
#include "MsgBase.hpp"
#include "FreeRTOS.h"
#include "queue.h"

namespace CHASSIS {
enum class FSMState_e : uint8_t;
}

namespace GIMBAL {
enum class FSMState_e : uint8_t;
}

namespace ARM {
struct Msg_s;
}

struct MsgBus_s {
    QueueHandle_t chassisQueue;
    QueueHandle_t gimbalQueue;
    QueueHandle_t armQueue;
    QueueHandle_t tpQueue;
};

//  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ ctrl msg ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
struct ChassisMsg_s : public Msg {
    CHASSIS::FSMState_e state;
    float vx, vy, yaw;
};

struct GimbalMsg_s : public Msg {
    GIMBAL::FSMState_e state;
    float roll, pitch, yaw;
};


//  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ other msg ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
struct GimbalEularMsg_s : public Msg {
    float roll, pitch, yaw;
};

struct DeltaYawMsg_s : public Msg {
    float deltaYaw;
};

//  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ temp cache ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
struct ChassisTxMsg_s {
    CHASSIS::FSMState_e state;
};

struct GimbalTxMsg_s {};

struct ArmTxMsg_s {};

struct RefereeTxMsg_s {};
