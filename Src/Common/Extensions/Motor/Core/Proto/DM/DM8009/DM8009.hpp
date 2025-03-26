#pragma once

#include "../../ProtocolPolicy.hpp"
#include "../DMCanGroup.hpp"

namespace PINYMOTOR {
struct DM8009Proto : MotorProto<DM8009Proto> {
    enum class Mode_e { MIT, POS_VEL, SPEED, EMIT };
    static constexpr uint32_t MOTORS_PER_FRAME = 1;
    using GroupType = DMGroup<DM8009Proto>;

    union CommandData_s {
        struct {
            float position, velocity;
        } posVel;
        struct {
            float speed;
        } speed;
        struct {
            float torque;
        } mit;
    };

    static uint32_t getCanId(Mode_e mode)
    {
        switch (mode) {
        case DM8009Proto::Mode_e::POS_VEL:
            return 0x100;
        case DM8009Proto::Mode_e::SPEED:
            return 0x200;
        case DM8009Proto::Mode_e::MIT:
            return 0x300;
        default:
            return 0;
        }
    }
    static void parseFeedback(class Motor<DM8009Proto> *motor, const uint8_t *data);
};
template class Motor<DM8009Proto>;
}
