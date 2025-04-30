#pragma once

#include <iostream>

enum class FSMMode_e {
    NORMAL,
    CHANGE,
    PAUSE,
};

class FSMState {
public:
    FSMState() = default;
    virtual ~FSMState() = default;

    auto update() -> void;

    virtual void enter() = 0;
    virtual void run() = 0; // TODO: some parameters to run
    virtual void exit() = 0;

    inline virtual FSMState *checkChange()
    {
        return nullptr;
    }; // return the next state ptr or nullptr

    std::string getStateName() { return stateName; };

private:
    std::string stateName;
    FSMMode_e mode_ = FSMMode_e::NORMAL;

    FSMState *nextState_ = nullptr;
    // current state is this
};
