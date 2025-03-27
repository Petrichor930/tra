#include "main.h"
#include "AppTask.hpp"

int main()
{
    HAL_Init();
    SystemClock_Config();
    PeriphCommonClock_Config();

    MX_GPIO_Init();
    MX_DMA_Init();
    MX_BDMA_Init();
    MX_FDCAN1_Init();
    MX_FDCAN2_Init();
    MX_FDCAN3_Init();
    MX_USART1_UART_Init();
    MX_TIM1_Init();
    MX_USART2_UART_Init();
    MX_SPI2_Init();
    MX_UART5_Init();
    MX_UART7_Init();
    MX_TIM2_Init();
    MX_TIM12_Init();
    MX_TIM6_Init();
    MX_USART10_UART_Init();
    MX_TIM13_Init();
    MX_TIM7_Init();
    MX_TIM3_Init();
    MX_ADC1_Init();
    MX_SPI6_Init();
    MX_UART9_Init();
    MX_USB_OTG_HS_PCD_Init();
    /* USER CODE BEGIN 2 */
    robot_init();
    /* USER CODE END 2 */
}
