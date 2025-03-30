#include "AppTask.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include "rc_control.hpp"
#include "main.h"

using namespace RC;

RemoteController rc;


extern UART_HandleTypeDef huart5;
extern FDCAN_HandleTypeDef hfdcan1;

void rcTask();

void robot_init()
{
    rc.init(&huart5);

    xTaskCreate((TaskFunction_t)rcTask, "rc_task", 256, NULL, 5, NULL);
    vTaskStartScheduler();
}


void rcTask()
{
    for (;;) {
        rc.update();
    }
}
