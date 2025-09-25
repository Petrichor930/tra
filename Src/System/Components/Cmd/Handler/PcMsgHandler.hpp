#pragma once

#include <cstdint>
#include "Handler.hpp"

#define PC_READY_EVENT (1 << 3)

class PcMsgHandler : public Handler {
#pragma pack(1)

    struct SendPacket_s {
        uint8_t header = 0xa5; // 1byte
        uint8_t task;          // 1byte
        float joints[7];       // 28byte
        uint16_t checksum;     // 2byte
    }; // 32byte

    struct ReceivePacket_s {
        uint8_t header = 0x5a;
        uint8_t state;
        float joints[7];
        uint16_t checksum;
    };
#pragma pack()

public:
    void init(MsgBus_s *_bus, EventGroupHandle_t _event) override;
    void handle() override;
    void notify(Msg *_msg, QueueHandle_t _queue) override;
    void parse(uint8_t *_data, uint32_t _len);

private:
    MsgBus_s *msgBus_;
    EventGroupHandle_t event_;
};
