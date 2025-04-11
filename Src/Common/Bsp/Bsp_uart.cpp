#include "Bsp_uart.hpp"

Uart *Uart::instance = new Uart();

void Uart::registerCallback(UART_HandleTypeDef *_huart, callback _pCallback)
{
    HAL_UART_RegisterRxEventCallback(_huart, _pCallback);
}

HAL_StatusTypeDef Uart::RecvDmaMultiBufInit(UART_HandleTypeDef *_huart,
                                            uint32_t *_dstAddress,
                                            uint32_t _dataLength)
{
    HAL_StatusTypeDef result = HAL_OK;
#if defined(SOC_MULTI_BUFFER)
    _huart->ReceptionType = HAL_UART_RECEPTION_TOIDLE;
    _huart->RxEventType = HAL_UART_RXEVENT_IDLE;
    _huart->RxXferSize = _dataLength;
    SET_BIT(_huart->Instance->CR3, USART_CR3_DMAR);
    __HAL_UART_ENABLE_IT(_huart, UART_IT_IDLE);
    uint32_t *secondMemAddress = _dstAddress + (_dataLength / 2);
    result = HAL_DMAEx_MultiBufferStart(
            _huart->hdmarx, (uint32_t)&_huart->Instance->RDR,
            (uint32_t)_dstAddress, (uint32_t)secondMemAddress, _dataLength);
#else
    _huart->pRxBuffPtr = (uint8_t *)_dstAddress;
    _huart->RxXferSize = _dataLength;
    _huart->ErrorCode = HAL_UART_ERROR_NONE;
    /* Enable the DMA Stream */
    HAL_DMA_Start(_huart->hdmarx, (uint32_t)&_huart->Instance->DR,
                  (uint32_t)_dstAddress, _dataLength);
    SET_BIT(_huart->Instance->CR3, USART_CR3_DMAR);
    __HAL_UART_CLEAR_OREFLAG(_huart);
    __HAL_UART_CLEAR_IDLEFLAG(_huart);
    __HAL_UART_ENABLE_IT(_huart, UART_IT_IDLE);
#endif
    return result;
}

HAL_StatusTypeDef Uart::RecvDmaInit(UART_HandleTypeDef *_huart,
                                    uint32_t *_dstAddress, uint32_t _dataLength)
{
    HAL_StatusTypeDef result = HAL_OK;
    __HAL_UART_ENABLE_IT(_huart, UART_IT_IDLE);
    result = HAL_UARTEx_ReceiveToIdle_DMA(_huart, (uint8_t *)&_dstAddress,
                                          _dataLength);
    __HAL_DMA_DISABLE_IT(_huart->hdmarx, DMA_IT_HT);
    return result;
}
