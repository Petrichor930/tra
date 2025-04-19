#include "AppManager.hpp"

#include "cmsis_os2.h"

#include "rc_control.hpp"

void rcTask()
{
    for (;;) {
        static RC::RemoteController rc;
        rc.update();
    }
}

void AppManager::createApp()
{
    xTaskCreate((TaskFunction_t)rcTask, "rc_task", 256, NULL,
                osPriorityRealtime, NULL);
}
