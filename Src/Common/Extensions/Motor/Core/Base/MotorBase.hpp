#pragma once
#include "projdefs.h"
#include <cstdint>

#include "../../../../System/RTOS/FreeRTOS/include/FreeRTOS.h"
#include "../../../../System/RTOS/FreeRTOS/include/task.h"
#include "../../../../System/RTOS/FreeRTOS/include/semphr.h"

#include <queue>

namespace PINYMOTOR {

enum class ComType_e { NONE, FDCAN, CAN, UART };
enum class WorkMode_e { QUAD_CURR, QUAD_VOLT, MIT_TT, MIT_VDES, MIT_VDESPDES, PDESVDES, VDES, EMIT };

enum class GlobalState_e { UNREGISTER, OFFLINE, ONLINE, ERROR };

enum class MotorErrorCode_e { ErrorNone = 0u };
typedef struct _InitConfig {
    uint32_t *pComHandle;
    ComType_e comType;
    WorkMode_e workMode;
    uint8_t offsetId;
    float txFreq;
} InitConfig_s;

typedef struct _Model_s {
    char name[10]; // 电机名称
    uint16_t measureMin; // 测量最小值
    uint16_t measureMax; // 测量最大值
    float reductionRatio; // 减速比
    uint16_t txBaseId; // 发送基ID
    uint16_t rxBaseId; // 接收基ID
} Model_s;

typedef struct _Data_s {
    int32_t rawScale; // 原始比例
    int32_t lastRawScale; // 上一次原始比例
    float singleCirAng; // rad
    float MultipCirAng; // rad
    float spdRadps;     // rad/s
    float spdRpm;       // rpm
    float curr;         // A
    float torq;         // Nm
    float tempture;     // °C
    float feedbackFreq; // Hz
}Data_s;


// 模板类，用于定义一拖一电机的基类
template <typename Derived> class MotorBase {
protected:
    Model_s model_;
    Data_s data_;

    uint32_t *pComHandle_;
    ComType_e comType_;
    WorkMode_e workMode_;
    GlobalState_e globalState_;
    uint8_t offsetId_;
    float txFreq_;

    bool isQuad_ = false; // default is not quad encoder

    std::queue<MotorErrorCode_e> errQueue_;

    uint32_t lastSendTick = 0; // ms

    // 定义发送互斥信号量
    static SemaphoreHandle_t sendMutex_;

    // 定义派生类对象
    Derived &derived() { return static_cast<Derived &>(*this); }
    const Derived &derived() const { return static_cast<const Derived &>(*this); }

public:
    // 构造函数
    MotorBase(InitConfig_s _config)
            : model_() // 显式初始化
            , data_()  // 显式初始化
            , pComHandle_(_config.pComHandle)
            , comType_(_config.comType)
            , workMode_(_config.workMode)
            , globalState_(GlobalState_e::UNREGISTER)  // 需明确初始化
            , offsetId_(_config.offsetId)
            , txFreq_(_config.txFreq)
    {
        // Base class constuctor
    }

    // 解析接收到的数据
    void parse(uint8_t *_rxBuffer) { derived().parse(_rxBuffer); }

    // 控制电机
    bool ctrl() { return derived().ctrl(); }

    // 设置电机扭矩
    void setTorq(float _torq) { derived().setTorq(_torq); };
    // 设置电机速度
    void setSpeed(float _speed) { derived().setSpeed(_speed); };
    // 设置电机位置
    void setPos(float _pos) { derived().setPos(_pos); };

    // 发送ID
    uint16_t canId() { return derived().canId(); }

    // 反馈ID
    uint16_t masterId() { return derived().masterId(); }

    // 唯一标识符
    uint16_t uid() { return derived().uid();}

    // 出轴减速比
    float RR(){ return model_.reductionRatio; }
    // 角度编码量程
    float span(){return static_cast<float>(model_.measureMax - model_.measureMin);}

    // 获取电机组ID
    uint32_t getGroupId() const { return 0; }

    // 检查是否需要发送数据
    bool checkSend(uint32_t _interval) const
    {
        return (xTaskGetTickCount() - lastSendTick) >= pdMS_TO_TICKS(_interval);
    }

    // 创建任务
    static void createTask();
};
template <typename Derived>
SemaphoreHandle_t MotorBase<Derived>::sendMutex_ = xSemaphoreCreateMutex();

// 模板类，用于定义一拖四电机的基类
template <typename Derived> class QuadMotorBase : public MotorBase<Derived> {
protected:
    // 定义一拖四电机的组大小为4
    static constexpr uint8_t QUAD_GROUP_SIZE_ = 4;

    // 使用基类的成员变量和成员函数
    using Base = MotorBase<Derived>;
    using Base::model_;

public:
    // 构造函数，初始化基类，并将isQuad_设置为true
    QuadMotorBase(InitConfig_s _config) : Base(_config) { this->isQuad_ = true; }
    // 获取电机的组ID
    uint32_t getGroupId() const override { return model_.txBaseId_; }
};

}
