#pragma once
#include <functional>
#include <thread>
#include <chrono>
#ifdef _DEBUG
#define INLINE inline
#else
#define INLINE __forceinline
#endif 
class Timer
{
public:

	Timer(int delay, std::function<void()> func) :
		m_Delay(delay), m_Func(func)
	{
		std::thread t([this]() {
			CoreThread();
		});
		t.detach();
	};
	virtual ~Timer() {};
	INLINE void Start() const 
	{
		m_Started = true;
	};
	INLINE void Start() 
	{
		m_Started = true;
	};
	INLINE void Stop() const
	{
		m_Started = false;
	};
	INLINE void Stop() 
	{
		m_Started = false;
	};
private:
	const int m_Delay;
	const std::function<void()> m_Func;
    mutable bool m_Started = false;
	INLINE void CoreThread() 
	{
		while (true)
		{
			if (m_Started) {
				m_Func();
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(m_Delay));
		}
		
	};
};
