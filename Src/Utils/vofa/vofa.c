#ifdef USE_VOFA

#include "vofa.h"
#include <stdint.h>


extern UART_HandleTypeDef VOFA_UART_HANDLE;
extern DMA_HandleTypeDef VOFA_DMA_TX_HANDLE;

/*
 * @brief 串口发送字符串 FireWater
 * @param format  Vofa_FireWater("a,b: %.2f,%.2f\n",)
 */
uint8_t vofa_FWBuffer[100] __attribute__((section(".ram_DMA")));
void Vofa_FireWater(const char *format, ...)
{
    uint32_t n;
    va_list args;
    va_start(args, format);
    n = vsnprintf((char *)vofa_FWBuffer, 100, format, args);
    HAL_UART_Transmit_DMA(&VOFA_UART_HANDLE, (uint8_t *)vofa_FWBuffer, n);
    va_end(args);
}

uint8_t vofa_JFBuffer[100] __attribute__((section(".ram_DMA")));
void Vofa_JustFloat(float *_data, uint8_t _num)
{
    uint32_t temp_end[4] = { 0, 0, 0x80, 0x7f };
    float temp_copy[_num];
    memcpy(&temp_copy, _data, _num * sizeof(float));
    memcpy(&vofa_JFBuffer[_num * 4], &temp_end[0], 4);
    HAL_UART_Transmit_DMA(&VOFA_UART_HANDLE, (uint8_t *)vofa_JFBuffer, (_num + 1) * 4);
}

#endif
