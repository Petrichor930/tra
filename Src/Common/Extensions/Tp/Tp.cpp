#include "TpMsg.hpp"
#include "Tp.hpp"
#include "main.h"
#include <cstdint>
#include <cstring>
#include "Crc.hpp"
#include "Bsp_uart.hpp"
#include "Bsp_dma.hpp"
#include "RefereeProt.hpp"

namespace TP {

Tp::Tp(UART_HandleTypeDef *_huart) : uart_(_huart)
{
    memset(&teachJoint_, 0, sizeof(teachJoint_));
}

void Tp::init(EventGroupHandle_t _event)
{
    event_ = _event;
    tpRxData = (VtTpFrame_t *)Dma::instance().ram_alloc(sizeof(VtTpFrame_t));

    uart_.recvDmaInit((uint8_t *)tpRxData, sizeof(VtTpFrame_t));
    uart_.registerCallback(
            [this](UART_HandleTypeDef *_huart, uint16_t _dataLength) {
                callBackFromISR(_huart, _dataLength);
            });
}


void Tp::callBackFromISR(UART_HandleTypeDef *_uart, uint16_t _pos)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(event_, TP_READY_EVENT, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
    HAL_UARTEx_ReceiveToIdle_DMA(_uart, (uint8_t *)tpRxData,
                                 sizeof(VtTpFrame_t));
    __HAL_DMA_DISABLE_IT(_uart->hdmarx, DMA_IT_HT); // NOLINT
}

void Tp::convert()
{
    if (Verify_CRC8_Check_Sum((uint8_t *)tpRxData,
                              sizeof(REFEREE::FrameHeader_s)) &&
        Verify_CRC16_Check_Sum((uint8_t *)tpRxData, sizeof(VtTpFrame_t))) {
        for (uint8_t i = 0; i < 7; i++) {
            teachJoint_.joint[i] = tpRxData->tpData.joint[i];
        }
    }
}

} // namespace TP
