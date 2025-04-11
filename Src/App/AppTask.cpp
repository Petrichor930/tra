#include "AppTask.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os2.h"
#include "rc_control.hpp"
#include "Soc.hpp"
#include "main.h"
#include "stm_log.hpp"

using namespace RC;
RemoteController rc;
extern UART_HandleTypeDef huart5;

extern canHandle hfdcan1;


void canCallback(canHandle *_hcan, const uint32_t &id, const uint8_t *canBuf)
{
    STM_LOGI("CAN RX: %d", id);
}

void rcTask()
{
    for (;;) {
        rc.update();
    }
}

void robotInit()
{
    osKernelInitialize();

    SEGGER_SYSVIEW_Conf();

    // Can *can = Can::getInstance();
    // can->registerCallback(&hfdcan1, 0x204, canCallback);
    // can->init(&hfdcan2, RX_FIFO0);

    xTaskCreate((TaskFunction_t)rcTask, "rc_task", 256, NULL,
                osPriorityRealtime, NULL);
    STM_LOGI("Robot Init");
    osKernelStart();
}
