#pragma once

#include "Soc.hpp"
#include HAL_INCLUDE
#include "BspBase.hpp"

class Tim : public BspBase<Tim> {
public:
    /**
     * @brief tim register callback
     */
    void registerCallback(TIM_HandleTypeDef *_htim,
                          HAL_TIM_CallbackIDTypeDef _callbackID,
                          pTIM_CallbackTypeDef _pCallback);
};
