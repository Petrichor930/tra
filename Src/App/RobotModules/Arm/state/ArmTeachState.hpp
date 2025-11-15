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

    void enter() override { LOG::info("Teach", "enter"); }

    bool change() override
    {
        Joint7D goal;
        for (uint8_t i = 0; i < 7; i++) {
            goal.j[i] = arm_.tpmsg_.target.j[i];
        }
        if (!arm_.motors.checkGoal(goal)) {
            LOG::error("Teach", "enter: init goal out of range");
            return false;
        }
        JointState_e initState = arm_.moveOneGoal(goal);

        if (initState == JointState_e::FINISH_STATE) {
            LOG::info("Teach", "enter: init positioning finished");
            return true;
        } else
            return false;
    }

    void run() override
    {
        for (uint8_t i = 1; i < 5; i++) {
            arm_.target_joints.j[i] = arm_.tpmsg_.target.j[i];
        }

        arm_.motors.safety.setSpeed(0.5);
        arm_.motors.safety.setAllAngleLimit(arm_.target_joints);
        arm_.motors.ctrl(arm_.target_joints); //output

        // if (arm_.msg_.pumpState == PUMP::State_e::ON) {
        //     arm_.pump.set(PUMP::Device_e::VALVE_2, PUMP::State_e::ON);
        // } else {
        //     arm_.pump.set(PUMP::Device_e::VALVE_2, PUMP::State_e::OFF);
        // }
    }

    void exit() override { LOG::info("Teach", "exit"); }

    FSMState_e checkChange() override
    {
        if (arm_.msg_.state == FSMState_e::STOP)
            return FSMState_e::STOP;
        else if (arm_.tpmsg_.state == FSMState_e::TEACH)
            return FSMState_e::TEACH;
        else if (arm_.msg_.state == FSMState_e::NORMAL)
            return FSMState_e::NORMAL;
        else if (arm_.msg_.state == FSMState_e::PLAN)
            return FSMState_e::PLAN;
        return FSMState_e::TEACH;
    }

private:
    Arm &arm_;
};

} // namespace ARM
