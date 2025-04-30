#include "FSMState.hpp"

auto FSMState::update() -> void
{
    if (mode_ == FSMMode_e::NORMAL) {
        this->run();
        nextState_ = this->checkChange();
        if (nextState_ != this) {
            mode_ = FSMMode_e::CHANGE;
        }
    } else if (mode_ == FSMMode_e::CHANGE) {
        this->exit();
        if (nextState_) {
            nextState_->enter();
        }
        mode_ = FSMMode_e::NORMAL;
    } else if (mode_ == FSMMode_e::PAUSE) {
        // do nothing
    } else {
        // error
    }
}
