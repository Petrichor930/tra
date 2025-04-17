#include "./MotorBase.hpp"

using namespace PINYMOTOR;

template<typename T>
MotorBase<T>::MotorBase(const char _name[16], InitConfig_s _config)
        : pComHandle_(_config.pComHandle)  // 通信句柄
        , comType_(_config.comType)  // 通信类型
        , workMode_(_config.workMode)  // 工作模式
        , globalState_(GlobalState_e::UNREGISTER)  // 需明确初始化
        , offsetId_(_config.offsetId)
{
    this->registerMotor(); // 实例创建即注册
    
    this->txFreq_ = _config.txFreq;
    strcpy(this->name_, _name);
    // Base class constuctor
}
