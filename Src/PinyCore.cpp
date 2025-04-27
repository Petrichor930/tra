#include "PinyCore.hpp"

#include "SEGGER_SYSVIEW.h"

#include "cmsis_os2.h"

#include "stm_log.hpp"

#include "AppManager.hpp"

// #include "../Lib/TinyMPC/src/tinympc/tiny_api.hpp"

static void logCallback(const char* _type, const char* _color, const char* _format, ...) {
    va_list args;
    va_start(args, _format);
    logProtoVaList(_type, _color, _format, args);
    va_end(args);
}

void PinyCore::init()
{
    osKernelInitialize();

    SEGGER_SYSVIEW_Conf();

    // Can *can = Can::inst();
    // can->registerCallback(&hfdcan1, 0x204, canCallback);
    // can->init(&hfdcan2, RX_FIFO0);

    this->registerLogger(logCallback);

    AppManager::inst()->createApp();

    this->log("INFO", "green", "PinyCore init done.\n");

    osKernelStart();
}

void initPinyCore()
{
    PinyCore::inst()->init();
}
