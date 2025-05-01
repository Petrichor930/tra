#pragma once

#include "Soc.hpp"
#include HAL_INCLUDE
#include "BspBase.hpp"

class Pwm : public BspBase<Pwm> {
public:
    /**
     * @brief pwm init
     */
    void init(TIM_HandleTypeDef *_htim, uint32_t _channel);

    /**
     * @brief pwm start
     */
    void start();

    /**
     * @brief pwm stop
     */
    void stop();

    /**
     * @brief pwm set duty
     */
    void setDutyCycle(uint32_t _dutyCycle);

    /**
     * @brief pwm set frequency
     */
    void setFrequency(uint32_t _frequency);

private:
    TIM_HandleTypeDef *htim_;
    uint32_t channel_;
    uint32_t frequency_;
    uint32_t dutyCycle_;
};
