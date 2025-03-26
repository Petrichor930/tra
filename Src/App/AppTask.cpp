#include "AppTask.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include "rc_control.hpp"

using namespace RC;

RemoteController rc;




void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart == &huart5) {
        Rc *rc = Rc::getInstance();
        rc->idleHandleFromISR(huart);
    }
}

void rc_task();

void robot_init()
{
    rc.init(&huart5);
    xTaskCreate((TaskFunction_t)rc_task, "rc_task", 256, NULL, 5, NULL);
    vTaskStartScheduler();
}


void rc_task()
{
    for (;;) {
        rc.update();
    }
}
