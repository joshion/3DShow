#include "thread.h"

Thread::Thread()
	: m_thread(nullptr),
    m_bWorking(false),
    m_bStatus(false)
{
}

Thread::~Thread()
{
	stop();
}

void Thread::start()
{
	std::lock_guard<std::mutex> lock(m_mutexThread);
	if (m_thread == nullptr)
	{
        m_thread = new std::thread([&]() {
            {
                std::lock_guard<std::mutex> lock_working(m_mutexWorking);
                m_bWorking = true;
            }
            while (true)
            {
                /*
                等待触发
                */
                {
                    std::unique_lock<std::mutex> lock_status(m_mutexStatus);
                    while (!m_bStatus)
                    {
                        m_cvStatus.wait(lock_status);
                    }
                }

                run();

                /*
                判断是否需要结束线程
                */
                {
                    std::lock_guard<std::mutex> lock_working(m_mutexWorking);
                    if (m_bWorking == false)
                    {
                        break;
                    }
                }

                {
                    std::unique_lock<std::mutex> lock_status(m_mutexStatus);
                    m_bStatus = false;
                }
            }
        });
	}
}

void Thread::stop()
{
    std::lock_guard<std::mutex> lock(m_mutexThread);
	if (m_thread)
	{
        {
            std::lock_guard<std::mutex> lock_working(m_mutexWorking);
            m_bWorking = false;
        }
        notifyThreadToContinue();
        if (m_thread)
		{
			m_thread->join();
			delete m_thread;
			m_thread = nullptr;
		}
	}
}

bool Thread::isWorking()
{
    std::lock_guard<std::mutex> lock_working(m_mutexWorking);
	return m_bWorking;
}

void Thread::notifyThreadToContinue()
{
    std::unique_lock<std::mutex> lock_status(m_mutexStatus);
    m_bStatus = true;
    m_cvStatus.notify_one();
}