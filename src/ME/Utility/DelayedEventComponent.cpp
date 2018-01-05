#include <Utility/DelayedEventComponent.hpp>
#include <iostream>

namespace me
{
	void DelayedEvent::fixedUpdate()
	{
		if (m_isRunning && ++m_framesElapsed >= m_delay)
		{
			m_event();

			if (--m_numRepeats >= 0)
			{
				m_framesElapsed = 0;
			}
			else
			{
				m_isRunning = false;
			}
		}
	}

	DelayedEvent::DelayedEvent(int delay, std::function<void()> function, int repeats) :
		m_delay(delay),
		m_numRepeats(repeats),
		m_event(function),
		m_framesElapsed(0),
		m_isRunning(true)
	{
	}

	DelayedEvent::~DelayedEvent()
	{
		//std::cout << "Delayed event destroyed" << std::endl;
	}
}