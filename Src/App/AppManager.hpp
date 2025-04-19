#pragma once


#ifdef __cplusplus
extern "C" {
#endif

class AppManager {
    // 单例模式
public:
    inline static AppManager *getInstance() { return &instance_; }

    // 禁止拷贝构造和赋值构造
    AppManager(const AppManager &) = delete;
    AppManager &operator=(const AppManager &) = delete;

    void start();
    void stop();
    void run();
    void update();
    void render();
    void input();
    void logic();
    void cleanup();

private:
    // 单例
    AppManager() = default;
    static AppManager instance_;
};

void robotInit();

#ifdef __cplusplus
}
#endif
