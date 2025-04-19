#pragma once

class AppManager {
public:
    inline static AppManager *inst() { return &instance_; }

    AppManager(const AppManager &) = delete;
    AppManager &operator=(const AppManager &) = delete;

    void createApp();

private:
    AppManager() = default;
    static AppManager instance_;
};
