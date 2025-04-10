#include "PinyCore.hpp"
#include "AppTask.hpp"

#include "FreeRTOS.h"

#include "task.h"
#include "cmsis_os2.h"
#include "rc_control.hpp"
#include "Soc.hpp"
#include "Bsp_can.hpp"
#include "stm_log.hpp"
#include <cstdint>


using namespace RC;

RemoteController rc;


extern UART_HandleTypeDef huart5;
extern canHandle hfdcan1;

void fdcan1_config(void)
{
    FDCAN_FilterTypeDef sFilterConfig;
    /* Configure Rx filter */
    sFilterConfig.IdType = FDCAN_STANDARD_ID; //标准ID，扩展ID不接收
    sFilterConfig.FilterIndex = 0;
    sFilterConfig.FilterType = FDCAN_FILTER_MASK;
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    sFilterConfig.FilterID1 = 0x00000000; //
    sFilterConfig.FilterID2 = 0x00000000; //
    if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK) {
        Error_Handler();
    }
    /* 全局过滤设置 */
    /* 接收到消息ID与标准ID过滤不匹配，不接受 */
    /* 接收到消息ID与扩展ID过滤不匹配，不接受 */
    /* 过滤标准ID远程帧 */
    /* 过滤扩展ID远程帧 */
    if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_REJECT, FDCAN_REJECT,
                                     FDCAN_FILTER_REMOTE,
                                     FDCAN_FILTER_REMOTE) != HAL_OK) {
        Error_Handler();
    }
    /* 开启RX FIFO0的新数据中断 */
    if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE,
                                       0) != HAL_OK) {
        Error_Handler();
    }
    /* Start the FDCAN module */
    if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK) {
        Error_Handler();
    }
}


void rcTask();

void canCallback(canHandle *_hcan, const uint32_t &id, const uint8_t *canBuf)
{
    STM_LOGI("CAN RX: %d", id);
}

void robotInit()
{
    osKernelInitialize();

    rc.init(&huart5);


    // SEGGER_SYSVIEW_Conf();
    Can* can = Can::getInstance();
    can->registerCallback(&hfdcan1, 0x204, canCallback);
    // can->init(&hfdcan2, RX_FIFO0);

    fdcan1_config();

    STM_LOGI("Robot Init");

    xTaskCreate((TaskFunction_t)rcTask, "rc_task", 256, NULL,
                osPriorityRealtime, NULL);
    osKernelStart();
}

void rcTask()
{
    for (;;) {
        rc.update();
    }
}
