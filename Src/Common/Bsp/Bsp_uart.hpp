#pragma once

#include "Soc.hpp"
#include HAL_INCLUDE

/**
 * @brief 串口双缓冲DMA接收初始化
 *
 * @param DataLength 请开辟两倍的缓冲区
 */
HAL_StatusTypeDef uartRecvDmaMultibufferInit(UART_HandleTypeDef *_huart,
                                             uint32_t *_dstAddress,
                                             uint32_t _dataLength);
/**
 * @brief 串口DMA接收初始化
 */
HAL_StatusTypeDef uartRecvDmaInit(UART_HandleTypeDef *_huart,
                                  uint32_t *_dstAddress, uint32_t _dataLength);
