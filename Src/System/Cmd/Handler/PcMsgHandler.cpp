#include "PcMsgHandler.hpp"
#include "Crc.hpp"
#include <cstring>

void PcMsgHandler::init(MsgBus_s *_bus, EventGroupHandle_t _event)
{
    this->msgBus_ = _bus;
    this->event = _event;
}

void PcMsgHandler::parse(uint8_t *_data, uint32_t _len)
{
    if (!Verify_CRC16_Check_Sum(_data, _len))
        return;
    ReceivePacket_s temp{};
    memcpy(&temp, _data, sizeof(ReceivePacket_s));
    xEventGroupSetBitsFromISR(event_, PC_READY_EVENT, nullptr);
}

void PcMsgHandler::handle() { SendPacket_s packet{}; }

void PcMsgHandler::notify(Msg *_msg, QueueHandle_t _queue)
{
    xQueueSend(_queue, _msg, 0);
}
