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

// binding motorMap_
template <typename T>
std::vector<std::pair<uint32_t *,
                      std::unordered_map<uint16_t, QuadMotorGroup_s *> > >
        QuadMotorBase<T>::motorMap_ = {};

template <typename T>
QuadMotorBase<T>::QuadMotorBase(const char _name[16], InitConfig_s _config) : Base(_name, _config)
{
    this->isQuad_ = true;
    // 注册电机到motorMap_中
    // 先寻找是否存在对应的pComHandle_
    auto it = std::ranges::find_if(motorMap_.begin(), motorMap_.end(),
                            [_config](const auto &pair) {
                                return pair.first == _config.pComHandle;
                            }); // lamda
    if (it == motorMap_.end()) {
        // 如果不存在，则直接在motorMap_尾部增多一个pair对象
        motorMap_.emplace_back(_config.pComHandle, std::unordered_map<uint16_t, QuadMotorGroup_s *>());
        it = motorMap_.end() - 1;
    }
    // 在找到的pair对象中添加电机
    auto &map = it->second;
    map[getGroupId()] = new QuadMotorGroup_s();
    map[getGroupId()]->motor[getPosInGroup()] = this;

    // 检查电机组中所有电机的发送频率是否一致
    for (size_t i = 0; i < 4; i++) {
        if (map[getGroupId()]->motor[i] != nullptr) {
            if (map[getGroupId()]->motor[i]->txFreq_ != this->txFreq_) {
                this->log("WARN", "", "Motor %s: txFreq not match", this->name_);
                return;
            }
        }
    }
}
