#include "IMotor.hpp"

#include "MotorManager.hpp"

using namespace PINYMOTOR;

MotorTypeDef_e IMotor::registerMotor()
{
    MotorManager *motorManager = MotorManager::getInstance();
    auto it = motorManager->motors().find(uid());
    if (it != motorManager->motors().end()) {
        return false;
    }
    motorManager->motors().insert({ uid(), this });
    return 0;
}

MotorTypeDef_e IMotor::cancelMotor()
{
    MotorManager *motorManager = MotorManager::getInstance();
    auto it = motorManager->motors().find(uid());
    if (it == motorManager->motors().end()) {
        return false;
    }
    motorManager->motors().erase(it);
    return 0;
}
