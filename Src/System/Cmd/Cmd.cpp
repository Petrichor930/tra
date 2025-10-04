#include "Cmd.hpp"
#include "MsgImpl.hpp"
#include "StmLog.hpp"
#include "Tp.hpp"
#include "TpMsgHandler.hpp"
#include "sdkconfig.h"

Cmd::Cmd() : eventGroup_(xEventGroupCreate())
{
    msgBus_.chassisQueue = xQueueCreate(30, sizeof(ChassisMsg_s));
    msgBus_.gimbalQueue = xQueueCreate(30, sizeof(GimbalMsg_s));
    msgBus_.armQueue = xQueueCreate(30, sizeof(ARM::Msg_s));
    msgBus_.tpQueue = xQueueCreate(30, sizeof(ARM::Msg_s));
    msgBus_.refereeQueue = xQueueCreate(30, sizeof(RefereeMsg_s));

    rttHandler_.init(&msgBus_, eventGroup_);
    rcHandler_.init(&msgBus_, eventGroup_);
    tpHandler_.init(&msgBus_, eventGroup_);
    pcHandler_.init(&msgBus_, eventGroup_);
#if defined APP_USE_REFEREE
    refereeHandler_.init(&msgBus_, eventGroup_);
#endif

    xTaskCreate(Cmd::task, "cmd_task", 256, this, 1, nullptr);

    LOG::info("cmd", "init success");
}


void Cmd::parseMsg()
{
    EventBits_t xBits = xEventGroupWaitBits(eventGroup_, EVENT_MASK, pdTRUE,
                                            pdFALSE, portMAX_DELAY);

    if (xBits & RC_READY_EVENT) {
        rcHandler_.handle();
    }
    if (xBits & TP_READY_EVENT) {
        tpHandler_.handle(); //new add
    }
    if (xBits & RTT_READY_EVENT) {
        rttHandler_.handle();
    }
    if (xBits & PC_READY_EVENT) {
        pcHandler_.handle();
    }
#if defined APP_USE_REFEREE
    if (xBits & REFEREE_READY_EVENT) {
        refereeHandler_.handle();
    }
#endif
}

void Cmd::task(void *_param)
{
    auto instance = static_cast<Cmd *>(_param);
    for (;;) {
        instance->parseMsg();
    }
}
