#pragma once

#include <mutex>
#include <thread>

class Thread
{
public:
	Thread();
	virtual ~Thread();

	virtual void start();
	virtual void stop();
	bool isWorking();

protected:
	virtual void WorkingFunc() = 0;
	bool m_bWorkingSwitch;

private:
	std::mutex m_tMutex;
	std::thread *m_thread;
};
