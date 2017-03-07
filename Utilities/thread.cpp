#include "Thread.h"

Thread::Thread()
	: m_thread(nullptr)
	, m_bWorkingSwitch(false)
{
}

Thread::~Thread()
{
	stop();
}

void Thread::start()
{
	std::lock_guard<std::mutex> lock(m_tMutex);
	if (!m_thread && !m_bWorkingSwitch)
	{
		m_thread = new std::thread(std::bind(&Thread::WorkingFunc, this));
		if (m_thread)
			m_bWorkingSwitch = true;
	}
}

void Thread::stop()
{
	if (m_thread)
	{
		std::lock_guard<std::mutex> lock(m_tMutex);
		if (m_thread)
		{
			m_thread->join();
			delete m_thread;
			m_thread = NULL;
		}
		m_bWorkingSwitch = false;
	}
}

bool Thread::isWorking()
{
	return m_bWorkingSwitch;
}