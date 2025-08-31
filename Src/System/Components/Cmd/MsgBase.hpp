#pragma once

#include <cstdint>

enum class MsgType_e : uint8_t {
    CHASSIS = 0,
    ARM //new add
};

enum class ControlSource_e : uint8_t {
    NONE, // 无控制源
    RC,   // 遥控器控制
    TP    // 示教器控制
}; //added for control source identification

class Msg {
public:
    MsgType_e msgType;
    ControlSource_e source; // 新增：标记指令来源
    virtual ~Msg() = default;
};
