#include "Bsp_can.hpp"

#if defined(SOC_FDCAN)

#define DLC(n) FDCAN_DLC_BYTES_##n

struct dlc_table {
    uint8_t dlc;
    uint32_t bytes;
};

const dlc_table dlc[] = {
    { 0, DLC(0) },   { 1, DLC(1) },   { 2, DLC(2) },   { 3, DLC(3) },
    { 4, DLC(4) },   { 5, DLC(5) },   { 6, DLC(6) },   { 7, DLC(7) },
    { 8, DLC(8) },   { 12, DLC(12) }, { 16, DLC(16) }, { 20, DLC(20) },
    { 24, DLC(24) }, { 32, DLC(32) }, { 48, DLC(48) }, { 64, DLC(64) },
};

HAL_StatusTypeDef fdcanFilterInit(FDCAN_HandleTypeDef *_fdcan, uint8_t _fifo)
{
    HAL_StatusTypeDef result = HAL_OK;
    FDCAN_FilterTypeDef sFilterConfig;
    /* Configure Rx filter */
    sFilterConfig.IdType = FDCAN_STANDARD_ID;
    sFilterConfig.FilterIndex = 0;
    sFilterConfig.FilterType = FDCAN_FILTER_MASK;
    if (_fifo == 0) {
        sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
        result = HAL_FDCAN_ActivateNotification(
                _fdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
    } else if (_fifo == 1) {
        sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO1;
        result = HAL_FDCAN_ActivateNotification(
                _fdcan, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0);
    }
    sFilterConfig.FilterID1 = 0x00000000;
    sFilterConfig.FilterID2 = 0x00000000;
    result = HAL_FDCAN_ConfigFilter(_fdcan, &sFilterConfig);
    return result;
}

HAL_StatusTypeDef fdcanInit(FDCAN_HandleTypeDef *_fdcan, uint8_t _fifo,
                            void (*pCallback)(FDCAN_HandleTypeDef *_hFDCAN))
{
    HAL_StatusTypeDef result = HAL_OK;
    result = fdcanFilterInit(_fdcan, _fifo);
    result = HAL_FDCAN_ConfigGlobalFilter(
            _fdcan,               //fdcan FDCAN_Handle
            FDCAN_REJECT,         //拒绝所有不匹配的标准ID数据帧
            FDCAN_REJECT,         // 拒绝所有不匹配的扩展ID数据帧
            FDCAN_FILTER_REMOTE,  //过滤掉所有标准ID远程帧
            FDCAN_FILTER_REMOTE); //过滤掉所有扩展ID远程帧
    result = HAL_FDCAN_Start(_fdcan);
    return result;
}

HAL_StatusTypeDef fdcanTransmitData(FDCAN_HandleTypeDef *hfdcan, uint16_t stdid,
                                    uint8_t *tx_data, uint32_t len)
{
    HAL_StatusTypeDef result = HAL_OK;
    FDCAN_TxHeaderTypeDef tx_header = { 0 };
    tx_header.Identifier = stdid;
    tx_header.IdType = FDCAN_STANDARD_ID;
    tx_header.TxFrameType = FDCAN_DATA_FRAME;
    tx_header.DataLength = dlc[len].bytes;
    tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    tx_header.BitRateSwitch = FDCAN_BRS_OFF;
    tx_header.FDFormat = FDCAN_CLASSIC_CAN;
    tx_header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    tx_header.MessageMarker = 0;
    result = HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &tx_header, tx_data);
    return result;
}

HAL_StatusTypeDef fdcanTransmitBrsData(FDCAN_HandleTypeDef *hfdcan,
                                       uint16_t stdid, uint8_t *tx_data,
                                       uint32_t len)
{
    HAL_StatusTypeDef result = HAL_OK;
    FDCAN_TxHeaderTypeDef tx_header = { 0 };
    tx_header.Identifier = stdid;
    tx_header.IdType = FDCAN_STANDARD_ID;
    tx_header.TxFrameType = FDCAN_DATA_FRAME;
    tx_header.DataLength = dlc[len].bytes;
    tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    tx_header.BitRateSwitch = FDCAN_BRS_ON;
    tx_header.FDFormat = FDCAN_FD_CAN;
    tx_header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    tx_header.MessageMarker = 0;
    result = HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &tx_header, tx_data);
    return result;
}

#elif defined(SOC_CAN)
HAL_StatusTypeDef canFilterInit(CAN_HandleTypeDef *_can, uint8_t _fifo)
{
    CAN_FilterTypeDef can_filter_st;
    can_filter_st.FilterActivation = ENABLE;
    can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
    can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
    can_filter_st.FilterIdHigh = 0x0000;
    can_filter_st.FilterIdLow = 0x0000;
    can_filter_st.FilterMaskIdHigh = 0x0000;
    can_filter_st.FilterMaskIdLow = 0x0000;
    if (_can->Instance == CAN1) {
        can_filter_st.FilterBank = 0;
    } else if (_can->Instance == CAN1) {
        can_filter_st.SlaveStartFilterBank = 14;
        can_filter_st.FilterBank = 14;
    }
    if (_fifo == 0)
        can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;
    else if (_fifo == 1)
        can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO1;
    HAL_CAN_ConfigFilter(&_can, &can_filter_st);
    HAL_CAN_Start(&_can);
    HAL_CAN_ActivateNotification(&_can, CAN_IT_RX_FIFO0_MSG_PENDING);
}

HAL_StatusTypeDef canInit(CAN_HandleTypeDef *_can, bool _fifo)
{
    canFilterInit(_can, _fifo);
    __HAL_CAN_ENABLE_IT(hcan, CAN_IT_BUSOFF);
    CAN_INIT_FLAG |= HAL_CAN_Start(hcan); // start can
    return CAN_INIT_FLAG;
}

HAL_StatusTypeDef canTransmitData(CAN_HandleTypeDef *hcan, uint16_t stdid,
                                  uint8_t *tx_data, uint32_t len);
{
    CAN_TxHeaderTypeDef tx_header;
    uint32_t can_mailbox;
    tx_header.DLC = size;
    tx_header.IDE = CAN_ID_STD;
    tx_header.RTR = CAN_RTR_DATA;
    tx_header.StdId = stdid;
    tx_header.TransmitGlobalTime = DISABLE;
    return HAL_CAN_AddTxMessage(hcan, &tx_header, tx_data, &can_mailbox);
}

#endif
