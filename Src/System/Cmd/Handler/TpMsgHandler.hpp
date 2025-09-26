#pragma once
#include "Handler.hpp"
#include "Tp.hpp"
#include "MsgImpl.hpp"
#include "MsgBase.hpp"
#include "Arm.hpp"

class TpMsgHandler : public Handler {
public:
    void init(MsgBus_s *_bus, EventGroupHandle_t _event) override;
    void handle() override;
    void notify(Msg *_msg, QueueHandle_t _queue) override;

private:
    TP::TeachPendant &tp_ = TP::TeachPendant::instance();
    ARM::Msg_s armMsg_;
    MsgBus_s *msgBus_;
    bool teachModeActive = false; // 初始化：未激活
};
