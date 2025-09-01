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

class TeachState : public FSMState<FSMState_e> {
public:
    TeachState(Arm &_arm) : FSMState(FSMState_e::TEACH), arm_(_arm) {};

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
    }

    void exit() override { LOG::info("Teach", "exit"); }

    FSMState_e checkChange() override
    {
        if (arm_.msg_.state == FSMState_e::STOP) {
            return FSMState_e::STOP;
        } else if (arm_.msg_.state == FSMState_e::NORMAL) {
            return FSMState_e::NORMAL;
        } else if (arm_.tpmsg_.state == FSMState_e::TEACH) {
            return FSMState_e::TEACH;
        } else if (arm_.msg_.state == FSMState_e::PLAN) {
            return FSMState_e::PLAN;
        }
        return FSMState_e::STOP;
    }

private:
    Arm &arm_;
};

} // namespace ARM
