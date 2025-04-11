#pragma once

#define HAL_INCLUDE <stm32h7xx_hal.h>

#define SOC_MULTI_BUFFER

#define SOC_FDCAN
#define SOC_CAN_NUM (3)
#define canHandle   FDCAN_HandleTypeDef
#define canHeader   FDCAN_RxHeaderTypeDef
#define RX_FIFO0    FDCAN_RX_FIFO0
#define RX_FIFO1    FDCAN_RX_FIFO1
