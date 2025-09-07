#include "TpMsgHandler.hpp"
#include "Arm.hpp"
#include "Rc.hpp" // 获取遥控器状态判断是否在示教模式
#include "Pump.hpp"
#include "sdkconfig.h"
#include <cstdint>

namespace ARM {
enum class FSMState_e : uint8_t;
}

void TpMsgHandler::init(MsgBus_s *_bus, EventGroupHandle_t _event)
{
    msgBus_ = _bus;
    this->event = _event;
    // extern UART_HandleTypeDef EXTENSION_UART_HANDLE;
    // tp_.init(&EXTENSION_UART_HANDLE, _event);
}

void TpMsgHandler::handle()
{
    tp_.convert();
    TpCtrl_t tpData = tp_.getData();
    RC::RcRawMsg_t rcData = RC::Rc::instance().getData();


    // 直接更新 teachModeActive_，无需中间变量 isTeachMode
    if (rcData.rc.switchLeft == RC_SW_UP) {
        if (!teachModeActive && rcData.rc.ch1 == 660) {
            teachModeActive = true;
        } //首次激活后，即使右摇杆回位，TP 仍保持有效。
    } else {
        teachModeActive = false;
    } // tp can move arm only when left switch is up and chassis is moving

    // 合并 teachModeActive 的赋值，避免冗余
    teachModeActive = (rcData.rc.switchLeft == RC_SW_DOWN) ||
                      (rcData.rc.switchLeft == RC_SW_UP && teachModeActive);

    armMsg_.state = ARM::FSMState_e::STOP;

    if (teachModeActive) {
        armMsg_.state = ARM::FSMState_e::TEACH;

        for (uint8_t i = 0; i < 7; ++i) {
            armMsg_.target.j[0] = tpData.joint[0];
        }

        if (tpData.push == 1) {
            armMsg_.pumpState = PUMP::State_e::ON;
        } else {
            armMsg_.pumpState = PUMP::State_e::OFF;
        }
    }

    notify(&armMsg_, msgBus_->tpQueue);
}

void TpMsgHandler::notify(Msg *_msg, QueueHandle_t _queue)
{
    xQueueSend(_queue, _msg, 0); // 非阻塞发送
}
