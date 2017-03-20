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
    /*
    线程已经实现了无限循环
    run()中不能有无限循环,亦不能阻塞
    */
    virtual void run() = 0;

    void notifyThreadToContinue();

private:
	std::mutex m_mutexThread;
	std::thread *m_thread;
    
    std::mutex m_mutexWorking;
    bool m_bWorking;

    std::condition_variable m_cvStatus;
    std::mutex m_mutexStatus;
    bool m_bStatus;

};
