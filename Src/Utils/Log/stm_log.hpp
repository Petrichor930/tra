#pragma once

// #ifdef USE_LOG
#if 1
#include "SEGGER_RTT.h"

typedef enum {
    STM_LOG_NONE, /*!< No log output */
    STM_LOG_ERROR, /*!< Critical errors, software module can not recover on its own */
    STM_LOG_WARN, /*!< Error conditions from which recovery measures have been taken */
    STM_LOG_INFO, /*!< Information messages which describe normal flow of events */
    STM_LOG_DEBUG, /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
    STM_LOG_VERBOSE /*!< Bigger chunks of debugging information, or frequent messages which can potentially flood the output. */
} stm_log_level_t;

typedef int stm_err_t;

/* Definitions for error constants. */
#define STM_OK                   0 /*!< stm_err_t value indicating success (no error) */
#define STM_FAIL                 -1 /*!< Generic stm_err_t code indicating failure */

#define STM_ERR_NO_MEM           0x101 /*!< Out of memory */
#define STM_ERR_INVALID_ARG      0x102 /*!< Invalid argument */
#define STM_ERR_INVALID_STATE    0x103 /*!< Invalid state */
#define STM_ERR_INVALID_SIZE     0x104 /*!< Invalid size */
#define STM_ERR_NOT_FOUND        0x105 /*!< Requested resource not found */
#define STM_ERR_NOT_SUPPORTED    0x106 /*!< Operation or feature not supported */
#define STM_ERR_TIMEOUT          0x107 /*!< Operation timed out */
#define STM_ERR_INVALID_RSTMONSE 0x108 /*!< Received response was invalid */
#define STM_ERR_INVALID_CRC      0x109 /*!< CRC or checksum was invalid */
#define STM_ERR_INVALID_VERSION  0x10A /*!< Version was invalid */
#define STM_ERR_INVALID_MAC      0x10B /*!< MAC address was invalid */
#define STM_ERR_NOT_FINISHED     0x10C /*!< Operation has not fully completed */
#define STM_ERR_NOT_ALLOWED      0x10D /*!< Operation is not allowed */

#define STM_ERR_WIFI_BASE        0x3000 /*!< Starting number of WiFi error codes */
#define STM_ERR_MESH_BASE        0x4000 /*!< Starting number of MESH error codes */
#define STM_ERR_FLASH_BASE       0x6000 /*!< Starting number of flash error codes */
#define STM_ERR_HW_CRYPTO_BASE \
    0xc000 /*!< Starting number of HW cryptography module error codes */
#define STM_ERR_MEMPROT_BASE \
    0xd000 /*!< Starting number of Memory Protection API error codes */

void _stm_error_check_failed(stm_err_t rc, const char *file, int line,
                             const char *function, const char *expression)
        __attribute__((__noreturn__));

#define STM_ERROR_CHECK(x)                                       \
    do {                                                         \
        stm_err_t stm_rc_ = (x);                                 \
        if (unlikely(stm_rc_ != STM_OK)) {                       \
            _stm_error_check_failed(stm_rc_, __FILE__, __LINE__, \
                                    __ASSERT_FUNC, #x);          \
        }                                                        \
    } while (0)

#define LOG_PROTO(type, color, format, ...)                                    \
    SEGGER_RTT_printf(0, "  %s%s" format "\r\n%s", color, type, ##__VA_ARGS__, \
                      RTT_CTRL_RESET)

/* 清屏*/
#define LOG_CLEAR()      SEGGER_RTT_WriteString(0, "  " RTT_CTRL_CLEAR)

/* 无颜色日志输出 */
#define LOG(format, ...) LOG_PROTO("", "", format, ##__VA_ARGS__)

/* 有颜色格式日志输出 */
// #define LOGI(format,...) LOG_PROTO("INFO: ", RTT_CTRL_TEXT_BRIGHT_GREEN , format, ##__VA_ARGS__)
// #define LOGW(format,...) LOG_PROTO("WARN: ", RTT_CTRL_TEXT_BRIGHT_YELLOW, format, ##__VA_ARGS__)
// #define LOGE(format,...) LOG_PROTO("ERROR: ", RTT_CTRL_TEXT_BRIGHT_RED   , format, ##__VA_ARGS__)

#define STM_LOGI(format, ...) \
    LOG_PROTO("", RTT_CTRL_TEXT_BRIGHT_GREEN, format, ##__VA_ARGS__)
#define STM_LOGW(format, ...) \
    LOG_PROTO("", RTT_CTRL_TEXT_BRIGHT_YELLOW, format, ##__VA_ARGS__)
#define STM_LOGE(format, ...) \
    LOG_PROTO("", RTT_CTRL_TEXT_BRIGHT_RED, format, ##__VA_ARGS__)


#else

#define LOG_CLEAR()
#define STM_ERROR_CHECK(x)
#define LOG
#define LOGI
#define LOGW
#define LOGE
#define STM_LOGI
#define STM_LOGW
#define STM_LOGE

#endif

void Float2Str(char *str, float va);
