/**
 * @file observer.hpp
 * @brief 遥控器控制观察者类
 * @note  多线程：该类不能在多线程使用,如需多线程使用，应加入互斥锁
 *
 * @copyright SCNU-PIONEER (c) 2025-2026
 */
#pragma once

#include <vector>
#include "rc.hpp"
#include <algorithm>


namespace RC {

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void update(const rc_ctrl_t) = 0;
};


class RemoteController {
public:
    RemoteController() { rc_ = Rc::instance(); };

    void init(UART_HandleTypeDef *uart) { rc_->init(uart); };

    void addObserver(IObserver *observer) { observers.push_back(observer); }
    void removeObserver(IObserver *observer)
    {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    inline void notifyObservers(const rc_ctrl_t &data)
    {
        for (auto *observer : observers) {
            if (observer != nullptr) {
                observer->update(data);
            }
        }
    }

    inline void update()
    {
        rc_->parseData();
        // notifyObservers(rc_->getData());
    }


private:
    Rc *rc_;
    std::vector<IObserver *> observers;
};

}
