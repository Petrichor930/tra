#pragma once

#include "TpMsg.hpp"
#include "main.h"
#include <cstdint>
#include <FreeRTOS.h>
#include "event_groups.h"
#include "Bsp.hpp"


#define TP_READY_EVENT (1 << 2)

namespace TP {

class TP {
private:
    TpCtrl_t teachJoint_;
    VtTpFrame_t *tpRxData;
    EventGroupHandle_t event_;
    Uart uart_;

public:
    TP(UART_HandleTypeDef *_huart);
    void init(EventGroupHandle_t _event);

    TpCtrl_t &getData() { return teachJoint_; }

    void callBackFromISR(UART_HandleTypeDef *_huart, uint16_t _pos);
    static void rawCallBackFromISR(UART_HandleTypeDef *_huart, uint16_t _Pos);
    void convert();
};

} // namespace TP
