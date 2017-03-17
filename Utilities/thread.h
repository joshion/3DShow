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
    virtual void run() = 0;

private:
	std::mutex m_mutexThread;
	std::thread *m_thread;
    
    std::mutex m_mutexWorking;
    bool m_bWorking;
};
