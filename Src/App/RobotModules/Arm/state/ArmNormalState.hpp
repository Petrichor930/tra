/* 
 * @file   armNormalState.hpp
 * #@brief 机械臂正常工作状态，机械臂在此状态下可以通过遥控器执行正常的动作
 */
#pragma once

#include "Arm.hpp"
#include "ArmMotor.hpp"
#include "StmLog.hpp"

namespace ARM {

class NormalState : public FSMState<FSMState_e> {
public:
    NormalState(Arm &_arm) : FSMState(FSMState_e::NORMAL), arm_(_arm) {};

    void enter() override
    {
        arm_.motors.enable();
        LOG::info("Normal", "enter");
    }

    bool change() override
    {
        arm_.target_joints = arm_.motors.current_joints;
        arm_.motors.ctrl(arm_.target_joints);
        return arm_.motors.init();
    }

    void run() override
    {
        for (uint8_t i = 0; i < 7; i++) {
            arm_.target_joints.j[i] += arm_.msg_.target.j[i];
        }
        arm_.motors.safety.setSpeed(1);
        arm_.motors.safety.setAllAngleLimit(arm_.target_joints);
        //output
        arm_.motors.ctrl(arm_.target_joints);
    }

    void exit() override { LOG::info("Normal", "exit"); }

    FSMState_e checkChange() override
    {
        if (arm_.msg_.state == FSMState_e::STOP)
            return FSMState_e::STOP;
        else if (arm_.msg_.state == FSMState_e::NORMAL)
            return FSMState_e::NORMAL;
        else if (arm_.msg_.state == FSMState_e::TEACH)
            return FSMState_e::TEACH;
        else if (arm_.msg_.state == FSMState_e::PLAN)
            return FSMState_e::PLAN;
        return FSMState_e::STOP;
    }

private:
    Arm &arm_;
};

} // namespace ARM
