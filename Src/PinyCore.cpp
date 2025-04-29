#include "Soc.hpp"
#include "cmsis_os2.h"
#include "stm_log.hpp"
#include "Bsp_can.hpp"
#include "PinyCore.hpp"
#include "AppManager.hpp"
#include "SEGGER_SYSVIEW.h"


// #include "../Lib/TinyMPC/src/tinympc/tiny_api.hpp"

static void logCallback(const char *_type, const char *_color,
                        const char *_format, ...)
{
    va_list args;
    va_start(args, _format);
    logProtoVaList(_type, _color, _format, args);
    va_end(args);
}
void canCallback(const uint8_t *canBuf) { STM_LOGI("CAN RX"); }

extern canHandle hfdcan1;


void PinyCore::init()
{
    osKernelInitialize();

    SEGGER_SYSVIEW_Conf();


    Can::instance().registerCallback(&hfdcan1, 0x201, canCallback);
    Can::instance().init(&hfdcan1, RX_FIFO0);

    this->registerLogger(logCallback);

    AppManager::inst()->createApp();

    this->log("INFO", "green", "PinyCore init done.\n");

    osKernelStart();
}

void initPinyCore() { PinyCore::inst()->init(); }
