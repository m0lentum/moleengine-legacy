#ifndef DELAYED_EVENT_HPP
#define DELAYED_EVENT_HPP

#include <functional>

namespace me
{
	class DelayedEvent
	{
	private:

		bool m_isRunning;
		int m_numRepeats;
		int m_framesElapsed;
		int m_delay; // the time to wait, in frames
		std::function<void()> m_event;

	public:

		void fixedUpdate();

		DelayedEvent(int delay, std::function<void()> function, int repeats = 1);
		~DelayedEvent();
	};
}

#endif // DELAYED_EVENT_HPP