# Bsp Can
请使用以下函数注册fdcan接收新消息的回调函数
```
    HAL_FDCAN_RegisterCallback(_fdcan, HAL_FDCAN_RX_BUFFER_NEW_MSG_CB_ID,
                               pCallback);
```

# Bsp Uart
请使用以下函数注册uart回调函数
```
HAL_StatusTypeDef HAL_UART_RegisterRxEventCallback(UART_HandleTypeDef *huart, pUART_RxEventCallbackTypeDef pCallback)

typedef  void (*pUART_RxEventCallbackTypeDef)
(struct __UART_HandleTypeDef *huart, uint16_t Pos); /*!< pointer to a UART Rx Event specific callback function */
```
