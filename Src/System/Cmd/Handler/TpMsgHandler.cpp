#include "TpMsgHandler.hpp"
#include "Arm.hpp"
#include "Rc.hpp"
#include "Tp.hpp"
#include "Pump.hpp"
#include "sdkconfig.h"
#include <cstdint>


void TpMsgHandler::init(MsgBus_s *_bus, EventGroupHandle_t _event)
{
    msgBus_ = _bus;
    this->event = _event;
    tp = std::make_unique<TP::Tp>(&EXTENSION_UART_HANDLE);
    tp->init(_event);
}

void TpMsgHandler::handle()
{
    tp->convert();
    TpCtrl_t tpData = tp->getData();
    RC::RcRawMsg_t rcData = rc->getData();

    if (rcData.rc.switchLeft == RC_SW_UP) {
        if (!teachModeActive && rcData.rc.ch1 == 660) {
            teachModeActive = true;
        }
    } else {
        teachModeActive = false;
    } // tp can move arm only when left switch is up and chassis is moving

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
    xQueueSend(_queue, _msg, 0);
}
