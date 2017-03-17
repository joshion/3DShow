#include "thread.h"

Thread::Thread()
	: m_thread(nullptr)
	, m_bWorking(false)
{
}

Thread::~Thread()
{
	stop();
}

void Thread::start()
{
	std::lock_guard<std::mutex> lock(m_mutexThread);
	if (!m_thread && !m_bWorking)
	{
        m_thread = new std::thread([&]() {
            {
                std::lock_guard<std::mutex> lock_working(m_mutexWorking);
                m_bWorking = true;
            }
            while (true)
            {
                run();
                {
                    std::lock_guard<std::mutex> lock_working(m_mutexWorking);
                    if (m_bWorking == false)
                    {
                        break;
                    }
                }
            }
        });
	}
}

void Thread::stop()
{
	if (m_thread)
	{
		std::lock_guard<std::mutex> lock(m_mutexThread);

        {
            std::lock_guard<std::mutex> lock_working(m_mutexWorking);
            m_bWorking = false;
        }
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