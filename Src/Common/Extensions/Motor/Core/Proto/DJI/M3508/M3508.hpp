#pragma once

#include "../../ProtocolPolicy.hpp"
#include "../DJICanGroup.hpp"

namespace PINYMOTOR {
struct DJIM3508Proto : MotorProto<DJIM3508Proto> {
    enum class Mode_e { TORQUE, POS, VEL };
    static constexpr uint32_t MOTORS_PER_FRAME = 4;
    using GroupType = DJICanGroup<DJIM3508Proto>;

    union CommandData_s {
        struct {
            float torque[4];
        } torq;
        struct {
            float position[4];
        } pos;
        struct {
            float velocity[4];
        } vel;
    };

    static uint32_t getCanId(Mode_e mode) { return (mode == Mode_e::TORQUE) ? 0x200 : 0x100; }
    static void parseFeedback(class Motor<DJIM3508Proto> *motor, const uint8_t *data) {

    };
};
template class Motor<DJIM3508Proto>;
};
