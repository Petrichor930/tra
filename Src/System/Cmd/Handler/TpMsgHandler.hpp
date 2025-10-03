#pragma once
#include "Handler.hpp"
#include "Tp.hpp"
#include "Rc.hpp"
#include "MsgImpl.hpp"
#include "MsgBase.hpp"
#include "Arm.hpp"

class TpMsgHandler : public Handler {
public:
    TpMsgHandler(UART_HandleTypeDef *_rchuart, UART_HandleTypeDef *_tphuart);
    void init(MsgBus_s *_bus, EventGroupHandle_t _event) override;
    void handle() override;
    void notify(Msg *_msg, QueueHandle_t _queue) override;

private:
    RC::Rc rc_;
    TP::TP tp_;
    ARM::Msg_s armMsg_;
    MsgBus_s *msgBus_;
    bool teachModeActive = false; // 初始化：未激活
};
