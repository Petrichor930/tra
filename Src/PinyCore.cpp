#include "PinyCore.h"

#include "SEGGER_SYSVIEW.h"

#include "cmsis_os2.h"

#include "stm_log.hpp"

#include "AppManager.hpp"

static void logCallback(const char* type, const char* color, const char* format, ...) {
    va_list args;
    va_start(args, format);
    logProtoVaList(type, color, format, args);
    va_end(args);
}

void initPinyCore()
{
    osKernelInitialize();

    SEGGER_SYSVIEW_Conf();

    // Can *can = Can::inst();
    // can->registerCallback(&hfdcan1, 0x204, canCallback);
    // can->init(&hfdcan2, RX_FIFO0);

    registerLogger(logCallback);

    AppManager::inst()->createApp();

    STM_LOGI("PinyCore Init");

    osKernelStart();
}
