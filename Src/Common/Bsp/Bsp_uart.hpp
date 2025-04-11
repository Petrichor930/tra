#pragma once

#include "Soc.hpp"
#include HAL_INCLUDE

class Uart {
public:
    using callback = void(UART_HandleTypeDef *, uint16_t);
    /**
    * @brief fdcan registerCallback
    */
    void registerCallback(UART_HandleTypeDef *_huart, callback _pCallback);

    /**
    * @brief 串口双缓冲DMA接收初始化
    *
    * @param DataLength 请开辟两倍的缓冲区
    */
    HAL_StatusTypeDef RecvDmaMultiBufInit(UART_HandleTypeDef *_huart,
                                          uint32_t *_dstAddress,
                                          uint32_t _dataLength);
    /**
    * @brief 串口DMA接收初始化
    */
    HAL_StatusTypeDef RecvDmaInit(UART_HandleTypeDef *_huart,
                                  uint32_t *_dstAddress, uint32_t _dataLength);

    /**
    * @brief fdcan rx callbackFromISR
    */
    inline void callbackFromISR(UART_HandleTypeDef *_huart, uint16_t _pos);

    /**
    * @brief fdcan get Instance
    */
    inline static Uart *getInstance() { return instance; }

private:
    static Uart *instance;
};
