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
        // 根据指令来源和状态切换
        if (arm_.msg_.state == FSMState_e::NORMAL) {
            if (arm_.msg_.source == ControlSource_e::RC) {
                return FSMState_e::NORMAL; // 切换到正常状态
            } else if (arm_.msg_.source == ControlSource_e::TP) {
                return FSMState_e::TEACH; // 切换到示教状态
            }
        } else if (arm_.msg_.state == FSMState_e::PLAN) {
            return FSMState_e::PLAN;
        }
        // 否则保持停止状态
        return FSMState_e::STOP;
    }


private:
    Arm &arm_;
};

} // namespace ARM
