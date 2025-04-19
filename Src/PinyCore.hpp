#pragma once

#include <utility>

class PinyCore {
    using LoggerCallback = void (*)(const char *, const char *, const char *,
                                    ...);
    static LoggerCallback activeLogger_;
public:
    inline static PinyCore *inst() { return &instance_; }

    PinyCore(const PinyCore &) = delete;
    PinyCore &operator=(const PinyCore &) = delete;

    inline void registerLogger(LoggerCallback logger) {
        activeLogger_ = std::move(logger);
    }

    template<typename... Args>
    inline void log(const char* type, const char* color, const char* format, Args... args) {
        if (activeLogger_) {
            activeLogger_(type, color, format, args...);
        }
    }

    // void bspInit();
    // void osInit();
    // void coreInit();

    void init();

private:
    PinyCore() = default;
    static PinyCore instance_;
};

#ifdef __cplusplus
extern "C" {
#endif
void initPinyCore();
#ifdef __cplusplus
}
#endif
