#include <Utility/TimerSystem.hpp>
#include <Utility/DelayedEventComponent.hpp>
#include <Component.hpp>
#include <Space.hpp>
#include <iostream>

namespace me
{
	void TimerSystem::continuousUpdate(sf::Time timeElapsed)
	{
		
	}

	void TimerSystem::fixedUpdate()
	{
		m_space->each<DelayedEvent>([](Component<DelayedEvent> &comp)
		{
			comp->fixedUpdate();
		});
	}
}