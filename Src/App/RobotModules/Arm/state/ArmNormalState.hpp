/* 
 * @file   armNormalState.hpp
 * #@brief 机械臂正常工作状态，机械臂在此状态下可以通过遥控器执行正常的动作
 */
#pragma once

#include "Arm.hpp"
#include "ArmMotor.hpp"

namespace ARM {

class NormalState : public FSMState<FSMState_e> {
public:
    NormalState(Arm &_arm) : FSMState(FSMState_e::PLAN), arm_(_arm) {};

    void enter() override { LOG::info("Normal", " enter"); }

    void run() override
    {
        Joint7D target = { arm_.msg_.j1, arm_.msg_.j2, arm_.msg_.j3,
                           arm_.msg_.j4, arm_.msg_.j5, arm_.msg_.j6,
                           arm_.msg_.j7 };
        //UT缓启动
        arm_.motors.setUTsmoothStart();
        arm_.safety.setSpeed(1);
        arm_.safety.setAllAngleLimit(target);
        //output
        arm_.motors.ctrl(target);
    }

    void exit() override { LOG::info("Normal", " exit"); }

    FSMState_e checkChange() override
    {
        if (arm_.msg_.state == FSMState_e::STOP) {
            return FSMState_e::STOP;
        } else if (arm_.msg_.state == FSMState_e::NORMAL) {
            return FSMState_e::NORMAL;
        } else if (arm_.msg_.state == FSMState_e::PLAN) {
            return FSMState_e::PLAN;
        } else if (arm_.tpmsg_.state == FSMState_e::TEACH) {
            return FSMState_e::TEACH;
        }
        return FSMState_e::STOP;
    }

private:
    Arm &arm_;
};

} // namespace ARM
