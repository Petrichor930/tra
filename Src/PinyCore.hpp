#pragma once

class PinyCore {
public:
    inline static PinyCore *inst() { return &instance_; }

    PinyCore(const PinyCore &) = delete;
    PinyCore &operator=(const PinyCore &) = delete;

    // void bspInit();
    // void osInit();
    // void coreInit();

    void init();

private:
    PinyCore() = default;
    static PinyCore instance_;
};

