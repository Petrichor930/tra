#pragma once

#include "Arm.hpp"
#include "FSMState.hpp"

namespace ARM {

class StopState : public FSMState<FSMState_e> {
public:
    StopState(Arm &_arm) : FSMState(FSMState_e::STOP), arm_(_arm) {};

    void enter() override
    {
        arm_.motors.stop();
        LOG::info("Stop", "enter");
    }

    void run() override
    {
        // arm_.motors.stop();
        }

    void exit() override { LOG::info("Stop", "exit"); }

    FSMState_e checkChange() override
    {
        if (arm_.msg_.state == FSMState_e::NORMAL) {
            return FSMState_e::NORMAL;
        } else if (arm_.msg_.state == FSMState_e::PLAN) {
            return FSMState_e::PLAN;
        } else if (arm_.tpmsg_.state == FSMState_e::TEACH) {
            return FSMState_e::TEACH;
        } else if (arm_.msg_.state == FSMState_e::STOP) {
            return FSMState_e::STOP;
        }
        return FSMState_e::STOP;
    }


private:
    Arm &arm_;
};

} // namespace ARM
