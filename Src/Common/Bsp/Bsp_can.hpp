#pragma once

#include "Soc.hpp"
#include HAL_INCLUDE

#if defined(SOC_FDCAN)
/**
 * @brief fdcan初始化并配置滤波器，不过滤任何ID
 */
HAL_StatusTypeDef fdcanInit(FDCAN_HandleTypeDef *_fdcan, uint8_t _fifo,
                            void (*_cb)(void));
/**
 * @brief fdcan发送普通数据帧
 */
HAL_StatusTypeDef fdcanTransmitData(FDCAN_HandleTypeDef *hfdcan, uint16_t stdid,
                                    uint8_t *tx_data, uint32_t len);
/**
 * @brief fdcan发送可变波特率数据帧
 */
HAL_StatusTypeDef fdcanTransmitBrsData(FDCAN_HandleTypeDef *hfdcan,
                                       uint16_t stdid, uint8_t *tx_data,
                                       uint32_t len);
#elif defined(SOC_CAN)
/**
 * @brief can初始化并配置滤波器，不过滤任何ID
 */
HAL_StatusTypeDef canInit(CAN_HandleTypeDef *hcan, bool FIFO);
/**
 * @brief can发送普通数据帧
 */
HAL_StatusTypeDef canTransmitData(CAN_HandleTypeDef *hcan, uint16_t stdid,
                                  uint8_t *tx_data, uint32_t len);
#endif
