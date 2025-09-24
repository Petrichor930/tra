#include "ARMSafety.hpp"
#include "PidBasic.hpp"
#include "Projdefs.hpp"
#include "StmLog.hpp"


#include "sdkconfig.h"
#include "ArmMotor.hpp"
#include "DM4310.hpp"
#include "DM8009.hpp"
#include "UT8010_6.hpp"

#include "GM6020.hpp"
#include "MotorCommonMacros.hpp"

#include "UTMotorMsg.hpp"
#include <cstdint>

extern canHandle HCAN2;
extern canHandle HCAN3;
extern UART_HandleTypeDef UNITREE_UART;
extern DMA_HandleTypeDef UNITREE_DMA;

using namespace PINYMOTOR;
using namespace ARM;


IncrementalPid joint7PosPid(10.f, 0.0f, 1.f, 2.0f, 0.01f);
IncrementalPid joint7VelPid(0.00006f, 0.0005f, 0.f, 25.2f, 0.1f);

Motors::Motors() : joint1(0.002, 0), joint7(0.002, 0), safety(*this)
{
    jointInfos[0] = { .angle_min = -3.15f, .angle_max = 3.15f };
    jointInfos[1] = { .angle_min = -0.01f, .angle_max = 1.30f };
    jointInfos[2] = { .angle_min = -0.04f, .angle_max = 0.88f };
    jointInfos[3] = { .angle_min = -1.06f, .angle_max = 1.20f };
    jointInfos[4] = { .angle_min = -3.14f, .angle_max = 1.34f };
    jointInfos[5] = { .angle_min = -2.20f, .angle_max = 2.17f };
    jointInfos[6] = { .angle_min = -6.14f, .angle_max = 6.14f };

    InitConfig_s ut80106Config = { .pComHandle = reinterpret_cast<uint32_t *>(
                                           &UNITREE_UART),
                                   .comType = PINYMOTOR::ComType_e::RS485,
                                   .workMode = PINYMOTOR::WorkMode_e::EMIT,
                                   .offsetId = static_cast<uint8_t>(1),
                                   .txFreq = 500.0f,
                                   .posPID = nullptr,
                                   .velPID = nullptr,
                                   .torqPID = nullptr,
                                   .isReverse = true };

    InitConfig_s dmJointConf2 = { .pComHandle =
                                          reinterpret_cast<uint32_t *>(&HCAN2),
                                  .comType = PINYMOTOR::ComType_e::FDCAN,
                                  .workMode = PINYMOTOR::WorkMode_e::PDESVDES,
                                  .offsetId = static_cast<uint8_t>(2),
                                  .txFreq = 500.0f,
                                  .posPID = nullptr,
                                  .velPID = nullptr,
                                  .torqPID = nullptr,
                                  .isReverse = false };

    InitConfig_s dmJointConf3 = { .pComHandle =
                                          reinterpret_cast<uint32_t *>(&HCAN2),
                                  .comType = PINYMOTOR::ComType_e::FDCAN,
                                  .workMode = PINYMOTOR::WorkMode_e::PDESVDES,
                                  .offsetId = static_cast<uint8_t>(3),
                                  .txFreq = 500.0f,
                                  .posPID = nullptr,
                                  .velPID = nullptr,
                                  .torqPID = nullptr,
                                  .isReverse = true };

    InitConfig_s dmJointConf4 = { .pComHandle =
                                          reinterpret_cast<uint32_t *>(&HCAN2),
                                  .comType = PINYMOTOR::ComType_e::FDCAN,
                                  .workMode = PINYMOTOR::WorkMode_e::PDESVDES,
                                  .offsetId = static_cast<uint8_t>(4),
                                  .txFreq = 500.0f,
                                  .posPID = nullptr,
                                  .velPID = nullptr,
                                  .torqPID = nullptr,
                                  .isReverse = false };

    InitConfig_s dmJointConf5 = { .pComHandle =
                                          reinterpret_cast<uint32_t *>(&HCAN2),
                                  .comType = PINYMOTOR::ComType_e::FDCAN,
                                  .workMode = PINYMOTOR::WorkMode_e::PDESVDES,
                                  .offsetId = static_cast<uint8_t>(5),
                                  .txFreq = 500.0f,
                                  .posPID = nullptr,
                                  .velPID = nullptr,
                                  .torqPID = nullptr,
                                  .isReverse = false };

    InitConfig_s dmJointConf6 = { .pComHandle =
                                          reinterpret_cast<uint32_t *>(&HCAN2),
                                  .comType = PINYMOTOR::ComType_e::FDCAN,
                                  .workMode = PINYMOTOR::WorkMode_e::PDESVDES,
                                  .offsetId = static_cast<uint8_t>(6),
                                  .txFreq = 500.0f,
                                  .posPID = nullptr,
                                  .velPID = nullptr,
                                  .torqPID = nullptr,
                                  .isReverse = true };


    InitConfig_s gmConfig = { .pComHandle =
                                      reinterpret_cast<uint32_t *>(&HCAN3),
                              .comType = PINYMOTOR::ComType_e::CAN,
                              .workMode = PINYMOTOR::WorkMode_e::QUAD_CURR,
                              .offsetId = static_cast<uint8_t>(7),
                              .txFreq = 500.0f,
                              .posPID = &joint7PosPid,
                              .velPID = &joint7VelPid,
                              .torqPID = nullptr,
                              .isReverse = false };

    motors.utMotor =
            new UTMOTOR::UT80106("joint1", ut80106Config, &UNITREE_DMA);
    motors.dmMotor1 = new DMMOTOR::DM8009("joint2", dmJointConf2);
    motors.dmMotor2 = new DMMOTOR::DM8009("joint3", dmJointConf3);
    motors.dmMotor3 = new DMMOTOR::DM4310("joint4", dmJointConf4);
    motors.dmMotor4 = new DMMOTOR::DM4310("joint5", dmJointConf5);
    motors.dmMotor5 = new DMMOTOR::DM4310("joint6", dmJointConf6);
    motors.djMotor = new DJIMOTOR::GM6020("joint7", gmConfig);

    // for (int i = 0; i < 7; i++) {
    //     motors.all_motors[i]->setZeroAng(0);
    // }
}

bool Motors::init() { return homingUT(); }

void Motors::update()
{
    // 更新所有电机状态
    for (int i = 0; i < 6; i++) {
        /* singleCirAng range: 0 ~ 2PI, we need range: -pi ~ pi */
        if (motors.all_motors[i]->data().singleCirAng >= PI) {
            current_joints.j[i] =
                    motors.all_motors[i]->data().singleCirAng - 2 * PI;
        } else {
            current_joints.j[i] = motors.all_motors[i]->data().singleCirAng;
        }
    }
    /* gm6020 run multi cirang */
    current_joints.j[6] = motors.all_motors[7]->data().multipCirAng;
    /*平行四边形关系 - 确保关节3角度在安全范围内*/
    // biasJoint3Angle(); TODO:BUG
}

void Motors::stop()
{
    for (auto &motor : motors.all_motors) {
        motor->cmd(MotorCmdType_e::OFF);
    }
}

void Motors::enable()
{
    for (auto &motor : motors.all_motors) {
        motor->cmd(MotorCmdType_e::ON);
    }
}

void Motors::ctrl(const Joint7D &_target_joints)
{
    motors.utMotor->cmdPosVel(_target_joints.j[0] + unitreeAngleFix,
                              ref_speed.joint1);
    motors.all_motors[1]->cmdPosVel(_target_joints.j[1], ref_speed._[1]);
    motors.all_motors[2]->cmdPosVel(_target_joints.j[2], ref_speed._[2]);
    motors.all_motors[3]->cmdPosVel(_target_joints.j[3], ref_speed._[3]);
    motors.all_motors[4]->cmdPosVel(_target_joints.j[4], ref_speed._[4]);
    motors.all_motors[5]->cmdPosVel(_target_joints.j[5], ref_speed._[5]);
    motors.djMotor->cmdPos(_target_joints.j[6]);
}


void Motors::biasJoint3Angle()
{
    jointInfos[2].angle_min = joint3HighPoint(current_joints.j[1]);
    jointInfos[2].angle_max = joint3LowPoint(current_joints.j[1]);
}

float Motors::joint3HighPoint(float _target)
{
    return ((-0.8119f) * _target) - 1.05f;
}

float Motors::joint3LowPoint(float _target)
{
    if (_target > 0.68f) {
        return ((-1.08163f) * _target) + 0.7355f;
    } else {
        return 0.0f;
    }
}

bool Motors::homingUT()
{
    static bool utResetState = false;
    if (utResetState)
        return true;

    PINYMOTOR::UTMOTOR::TransmitMsg_s utTxMsg = {};

    motors.utMotor->setKd(0.03f);
    ref_speed.joint1 = -1.0f;

    if (fabs(motors.utMotor->data().torq) >= 0.37f &&
        fabs(motors.utMotor->data().spdRadps) < 0.1f) {
        unitreeAngleFix = motors.utMotor->data().multipCirAng;

        motors.utMotor->setKp(0.5f);
        motors.utMotor->setKd(0.f);
        ref_speed.joint1 = 0;

        motors.utMotor->setZeroAng();
        current_joints.j[0] = 0;
        utResetState = true;
        return true;
    } else {
        motors.utMotor->cmdVel(ref_speed.joint1);
        return false;
    }
}

bool Motors::checkGoal(Joint7D _goal)
{
    for (int i = 0; i < 2; i++) {
        if (!IS_WITHIN_RANGE(_goal.j[i], jointInfos[i].angle_min,
                             jointInfos[i].angle_max)) {
            LOG::error("ARM", "Joint%d move goal error", i + 1);
            return false;
        }
    }
    /*joint3*/
    float highTemp = joint3HighPoint(_goal.j[1]);
    float lowTemp = joint3LowPoint(_goal.j[1]);
    if (!IS_WITHIN_RANGE(_goal.j[2], highTemp, lowTemp)) {
        LOG::error("ARM", "Joint3 move goal error");
        return false;
    }
    /*joint4 - joint7*/
    for (int i = 3; i < 7; i++) {
        if (!IS_WITHIN_RANGE(_goal.j[i], jointInfos[i].angle_min,
                             jointInfos[i].angle_max)) {
            LOG::error("ARM", "Joint%d move goal error", i + 1);
            return false;
        }
    }
    return true;
} //checkGoal
