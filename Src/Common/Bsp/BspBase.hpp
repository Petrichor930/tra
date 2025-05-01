#pragma once

#include <stdint.h>
#include <functional>

template <typename Derived> class BspBase {
public:
    inline static Derived &instance()
    {
        static Derived instance;
        return instance;
    }

protected:
    BspBase(const BspBase &);
    BspBase &operator=(const BspBase &);
    BspBase() = default;
};
