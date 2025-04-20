#pragma once

class AppManager {
public:
    inline static AppManager* inst() {
        static AppManager instance;
        return &instance;
    }

    AppManager(const AppManager &) = delete;
    AppManager &operator=(const AppManager &) = delete;

    void createApp();

private:
    AppManager() = default;
};
