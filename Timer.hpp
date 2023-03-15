#pragma once
#include <functional>
#include <thread>
#include <chrono>
#ifdef _DEBUG
#define INLINE inline
#else
#define INLINE __forceinline
#endif 

class Timer {
public:
    Timer(int interval, std::function<void()> task)
        : m_interval(interval), m_task(task) {};
    virtual ~Timer() {};
    void Start()
    {
        if (!m_isRunning) {
            m_isRunning = true;
            m_thread = std::thread(&Timer::loop, this);
        }
    }

    void Stop()
    {
        if (m_isRunning) {
            m_isRunning = false;
            if (m_thread.joinable())
                m_thread.join();
        }
    }

private:
    int m_interval;
    std::function<void()> m_task;
    std::thread m_thread;
    bool m_isRunning=false;

    void loop() {
        while (m_isRunning) {
            m_task();
            std::this_thread::sleep_for(std::chrono::milliseconds(m_interval));
        }
    }
};
