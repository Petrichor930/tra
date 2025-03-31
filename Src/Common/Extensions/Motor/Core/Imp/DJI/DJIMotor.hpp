/*
 * @Author: MYUIN 2812090269@qq.com
 * @Date: 2025-03-29 23:07:03
 * @LastEditors: MYUIN 2812090269@qq.com
 * @LastEditTime: 2025-03-30 01:50:06
 * @FilePath: \PinyCore\Src\Common\Extensions\Motor\Core\Imp\DJI\DJIMotor.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include "../../Base/MotorBase.hpp"

#include <unordered_map>

namespace PINYMOTOR {

#pragma pack(push, 1)
typedef struct _DJIMotorMsg_s {
    int16_t cmd[4];
} DJIMotorMsg_s;
typedef struct _DJIMotorFeedback_s {
    uint16_t rawScale;
    int16_t rawRpm;
    int16_t current;
    uint8_t temperature;
} DJIMotorFeedback_s;
#pragma pack(pop)
typedef struct _DJIMotorStats_s {
    float currCodeSpan; 
    float currRated;    // A
    float torqRated;    // Nm
    float currMax;      // A
    float torqMax;      // Nm
    float torqConstant; // Nm/A

    _DJIMotorStats_s& operator=(const _DJIMotorStats_s& _other) {
        if (this != &_other)
        {
            currCodeSpan = _other.currCodeSpan;
            currRated = _other.currRated;
            torqRated = _other.torqRated;
            currMax = _other.currMax;
            torqMax = _other.torqMax;
            torqConstant = _other.torqConstant;
        }
        return *this;
    }
} DJIMotorStats_s;

template <typename Derived> class DJIMotor : public QuadMotorBase<DJIMotor<Derived>>{
    using Base = QuadMotorBase<DJIMotor<Derived> >;

protected:
    DJIMotorStats_s stats_;

    static std::unordered_map<int, DJIMotor*> motorMap;
    
};
}