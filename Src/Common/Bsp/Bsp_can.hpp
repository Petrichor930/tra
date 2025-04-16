#pragma once

#include "Soc.hpp"
#include <functional>
#include <unordered_map>
#include HAL_INCLUDE


class Can {
public:
    using callback =
            std::function<void(canHandle *, const uint32_t &, const uint8_t *)>;
    /**
     * @brief fdcan registerCallback
     */
    void registerCallback(canHandle *_hcan, uint32_t _fifo,
                          callback _pCallback);
    /**
    * @brief fdcan初始化并配置滤波器，不过滤任何ID
    */
    HAL_StatusTypeDef init(canHandle *_hcan, uint32_t _fifo);

    /**
    * @brief fdcan发送普通数据帧
    */
    HAL_StatusTypeDef transmitData(canHandle *_hcan, uint16_t _stdid,
                                   uint8_t *_txData, uint32_t _len);

    /**
    * @brief fdcan发送可变波特率数据帧
    */
    HAL_StatusTypeDef transmitBrsData(canHandle *_hcan, uint16_t _stdid,
                                      uint8_t *_txData, uint32_t _len);

    /**
    * @brief fdcan rx callbackFromISR
    */
    void callbackFromISR(canHandle *_hcan, uint32_t _rxFifo);

    /**
    * @brief fdcan get Instance
    */
    inline static Can *getInstance() { return instance; }

private:
    static Can *instance;
    std::unordered_map<canHandle *, std::unordered_map<uint32_t, callback> >
            cbTable;
};
