/**
 * @file statemachine.hpp
 *
 * @brief 状态机 
 * @note  请在外部定义具体的状态，实践，动作 
 *
 * @copyright SCNU-PIONEER (c) 2025-2026
 *
 */
#pragma once
#include <iostream>
#include <unordered_map>
#include <functional>
#include <string>
#include "stm_log.hpp"

namespace STATEMACHINE {
using State = std::string;
using Event = std::string;
using Action = std::function<void()>;

struct Transition {
    State nextState;
    Action action;
};

class StateMachine {
public:
    StateMachine(const State &initialState) : currentState(initialState) {}

    /** 注册状态转换规则
          * @param fromState 初始化状态
          * @event 初始化⌚
          * @tostate 下一状态
          * @action 执行动作
          */
    void addTransition(const State &fromState, const Event &event,
                       const State &toState, const Action &action = nullptr)
    {
        stateTable[fromState][event] = { toState, action };
    }

    // 处理事件
    inline void handleEvent(const Event &event)
    {
        auto &transitions = stateTable[currentState];
        auto it = transitions.find(event);
        if (it != transitions.end()) {
            const Transition &transition = it->second;
            if (transition.action) {
                transition.action(); // 执行动作
            }
            lastState = currentState;
            currentState = transition.nextState; // 更新状态
            STM_LOGI("state changed to: %s", currentState.c_str());
        } else {
            STM_LOGI("No transition defined for event: %s\n in state : %s",
                     event.c_str(), currentState.c_str());
        }
    }

    // 获取当前状态
    State getCurrentState() const { return currentState; }

private:
    State currentState;
    State lastState;
    std::unordered_map<State, std::unordered_map<Event, Transition> >
            stateTable; // 状态表
};
}
