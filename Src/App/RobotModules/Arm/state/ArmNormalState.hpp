/* 
 * @file   armNormalState.hpp
 * #@brief 机械臂正常工作状态，机械臂在此状态下可以通过遥控器执行正常的动作
 */
#pragma once

#include "Arm.hpp"
#include "ArmMotor.hpp"

namespace ARM {

class ArmNormalState : public FSMState<FSMState_e> {
public:
    ArmNormalState(Arm &_arm) : FSMState(FSMState_e::PLAN), arm_(_arm) {};

    void enter() override { LOG::info("Normal", " enter"); }

    void run() override
    {
        if (arm_.msg_.source == ControlSource_e::RC) {
            Joint7D target = { arm_.msg_.j1, arm_.msg_.j2, arm_.msg_.j3,
                               arm_.msg_.j4, arm_.msg_.j5, arm_.msg_.j6,
                               arm_.msg_.j7 };
            //UT缓启动
            arm_.motors.setUTsmoothStart();
            arm_.safety.setSpeed(1);
            arm_.safety.setAllAngleLimit(target);
            //output
            arm_.motors.ctrl(target);
        } else {
            // 来源不符时，停止运动（安全处理）
            arm_.motors.stop();
        }
    }

    void exit() override { LOG::info("Normal", " exit"); }

    FSMState_e checkChange() override
    {
        if (arm_.msg_.state == State_e::STOP) {
            return FSMState_e::STOP;
        }
        // 若指令来源为示教器且状态正常，切换到示教状态
        else if (arm_.msg_.state == State_e::TEACH &&
                 arm_.msg_.source == ControlSource_e::TP) {
            return FSMState_e::TEACH;
        } else if (arm_.msg_.state == State_e::PLAN) {
            return FSMState_e::PLAN;
        }
        // 保持正常状态
        else if (arm_.msg_.state == State_e::NORMAL) {
            return FSMState_e::NORMAL;
        }
        return FSMState_e::NORMAL;
    }

private:
    Arm &arm_;
};

} // namespace ARM
