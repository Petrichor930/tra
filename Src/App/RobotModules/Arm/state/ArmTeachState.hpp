/* 
 * @file   ArmTeachState.hpp
 * #@brief 机械臂示教状态，通过自定义控制器控制机械臂
 */

#pragma once

#include "Arm.hpp"
#include "ArmMotor.hpp"
#include "Pump.hpp"
#include "StmLog.hpp"

namespace ARM {

class ArmTeachState : public FSMState<FSMState_e> {
public:
    ArmTeachState(Arm &_arm) : FSMState(FSMState_e::TEACH), arm_(_arm) {};

    void enter() override
    {
        LOG::info("Teach", "enter");
        changingTime_.duration = 1000;

        Joint7D goal = { arm_.msg_.j1, arm_.msg_.j2, arm_.msg_.j3, arm_.msg_.j4,
                         arm_.msg_.j5, arm_.msg_.j6, arm_.msg_.j7 };

        if (!arm_.motors.checkGoal(goal)) {
            LOG::error("Teach", "enter: init goal out of range");
        }

        changingTime_.current++;

        Arm::JointState_e initState = arm_.moveOneGoal(goal);

        if (initState == Arm::JointState_e::FINISH_STATE) {
            LOG::info("Teach", "enter: init positioning finished");
        } else if (changingTime_.current <= changingTime_.duration) {
            LOG::info("Teach", "enter: positioning moving");
        } else if (changingTime_.current >= changingTime_.duration) {
            LOG::error("Teach", "enter: fail positioning moving");
        }
    }

    void run() override
    {
        if (arm_.msg_.source == ControlSource_e::TP) {
            Joint7D target = { arm_.msg_.j1, arm_.msg_.j2, arm_.msg_.j3,
                               arm_.msg_.j4, arm_.msg_.j5, arm_.msg_.j6,
                               arm_.msg_.j7 };

            arm_.target_joints = { arm_.msg_.j1, arm_.msg_.j2, arm_.msg_.j3,
                                   arm_.msg_.j4, arm_.msg_.j5, arm_.msg_.j6,
                                   arm_.msg_.j7 };
            arm_.safety.setSpeed(0.5);
            arm_.safety.setAllAngleLimit(arm_.target_joints);
            arm_.motors.ctrl(arm_.target_joints); //output


            if (arm_.msg_.pumpState == PUMP::State_e::ON) {
                arm_.pump.set(PUMP::Device_e::VALVE_2, PUMP::State_e::ON);
            } else {
                arm_.pump.set(PUMP::Device_e::VALVE_2, PUMP::State_e::OFF);
            }

        } else {
            // 来源不符时，停止运动（安全处理）
            arm_.motors.stop();
        }
    }

    void exit() override { LOG::info("Teach", "exit"); }

    FSMState_e checkChange() override
    {
        // 根据消息状态和来源判断是否切换状态
        if (arm_.msg_.state == FSMState_e::STOP) {
            return FSMState_e::STOP;
        }
        // 若指令来源为遥控器且状态正常，切换到正常状态
        else if (arm_.msg_.state == FSMState_e::NORMAL &&
                 arm_.msg_.source == ControlSource_e::RC) {
            return FSMState_e::NORMAL;
        }
        // 其他情况保持示教状态
        else if (arm_.msg_.state == FSMState_e::TEACH) {
            return FSMState_e::TEACH;
        }
        // 切换到规划状态
        else if (arm_.msg_.state == FSMState_e::PLAN) {
            return FSMState_e::PLAN;
        }
        return FSMState_e::TEACH;
    }

private:
    Arm &arm_;
};

} // namespace ARM
