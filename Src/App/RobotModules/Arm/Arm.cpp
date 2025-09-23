#include "Arm.hpp"
#include "ArmMotor.hpp"
#include "StmLog.hpp"
#include <memory>
#include "ArmNormalState.hpp"
#include "ArmStopState.hpp"
#include "ArmPlanState.hpp"
#include "ArmTeachState.hpp"
#include "MotorCommonMacros.hpp"
#include "Cmd.hpp"

using namespace ARM;

Arm::Arm() : joint0(0.002, 0), joint6(0.002, 0)
{
    /* FSM */
    stateFactory_.addState(FSMState_e::STOP,
                           std::make_unique<StopState>(*this));
    stateFactory_.addState(FSMState_e::NORMAL,
                           std::make_unique<NormalState>(*this));
    stateFactory_.addState(FSMState_e::PLAN,
                           std::make_unique<PlanState>(*this));
    stateFactory_.addState(FSMState_e::TEACH,
                           std::make_unique<TeachState>(*this));
    stateFactory_.init(stateFactory_.getNextState(FSMState_e::STOP));

    LOG::info("ARM", "register");

    motors.stop();
}

void Arm::update()
{
    extern Cmd *cmd;
    if (xQueueReceive(cmd->getMsgBus()->armQueue, &msg_, 0) == pdTRUE) {
        uint32_t i = 0;
    }
    // xQueueReceive(cmd.getMsgBus()->tpQueue, &tpmsg_, 0);
    motors.update();
    stateFactory_.update();
}

void Arm::setTargetPose(const JointRoute_s _route)
{
    RouteDta.target_pose = _route.pose;
    RouteDta.target_point = _route.point;
    RouteDta.pump = _route.pump;
}

void Arm::moveRoute()
{
    if (moveOneGoal(RouteDta.target_pose[RouteDta.point_cnt]) ==
        jointStateFlag) { //bug
        /*为了到达某点后停止一段时间*/
        if (RouteDta.target_pose[RouteDta.point_cnt].delay != 0) {
            LOG::info("ARM", "Start move delay");
            Dwt::instance().delayMs(
                    RouteDta.target_pose[RouteDta.point_cnt].delay);
            LOG::info("ARM", "End move delay");
        }
        RouteDta.point_cnt++;
        if (RouteDta.point_cnt == RouteDta.target_point) {
            RouteDta.point_cnt = 0;
            LOG::info("ARM", "Move all point done");
            //change state to normal
            msg_.state = FSMState_e::NORMAL;
        }
    }
}

JointState_e Arm::moveOneGoal(const Joint7D &_goal)
{
    /*第一次进入之后*/
    if (jointStateFlag == JointState_e::MOVING_STATE) {
        RouteDta.rateCnt++;
        float rate = static_cast<float>(RouteDta.rateCnt) / 2000.0f;

        target_joints.j[0] = joint0.update();
        target_joints.j[6] = joint6.update();
    }
    /*第一次进入*/
    if (jointStateFlag == JointState_e::FINISH_STATE) {
        /*joint1 - joint2*/
        for (int i = 0; i < 2; i++) {
            if (!IS_WITHIN_RANGE(_goal.j[i], motors.jointInfos[i].angle_min,
                                 motors.jointInfos[i].angle_max)) {
                LOG::error("ARM", "Joint%d move goal error", i + 1);
                return jointStateFlag;
            }
        }

        /*joint3*/
        float highTemp = motors.joint3HighPoint(_goal.j[1]);
        float lowTemp = motors.joint3LowPoint(_goal.j[1]);
        if (!IS_WITHIN_RANGE(_goal.j[2], highTemp, lowTemp)) {
            LOG::error("ARM", "Joint3 move goal error");
            return jointStateFlag;
        }

        /*joint4 - joint7*/
        for (int i = 3; i < 7; i++) {
            if (!IS_WITHIN_RANGE(_goal.j[i], motors.jointInfos[i].angle_min,
                                 motors.jointInfos[i].angle_max)) {
                LOG::error("ARM", "Joint%d move goal error", i + 1);
                return jointStateFlag;
            }
        }

        // 初始化目标关节值（除1和6外）
        for (uint8_t i = 1; i < 6; i++) {
            target_joints.j[i] = _goal.j[i];
        }

        joint0.syncPosition(motors.current_joints.j[0]);
        joint0.setTarget(_goal.j[0]);
        joint6.syncPosition(motors.current_joints.j[6]);
        joint6.setTarget(_goal.j[6]);

        target_joints.j[0] = motors.current_joints.j[0];
        target_joints.j[6] = motors.current_joints.j[6];

        Joint7D initDeltaJoints = _goal - motors.current_joints;
        float initMaxDeltaAngle = AbsMaxOf7(initDeltaJoints);

        float maxTime = initMaxDeltaAngle / DEFAULT_JOINT_SPEED;
        motors.safety.setJointSpeedLimit(maxTime, initDeltaJoints);
        motors.safety.setAllAngleLimit(target_joints);

        pump.apply(&RouteDta.pump[RouteDta.point_cnt]);

        jointStateFlag = JointState_e::MOVING_STATE;
    }
    Joint7D deltaJoints = _goal - motors.current_joints;
    float maxDeltaAngle = AbsMaxOf7(deltaJoints);
    if (maxDeltaAngle < 0.02f) {
        RouteDta.rateCnt = 0;
        jointStateFlag = JointState_e::FINISH_STATE;
        LOG::info("ARM", "Move point%d done", RouteDta.point_cnt);
    }

    // output
    motors.ctrl(target_joints);

    return jointStateFlag;
}
