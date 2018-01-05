#ifndef TIMER_SYSTEM_HPP
#define TIMER_SYSTEM_HPP

#include "../ISystem.hpp"
#include <SFML/System/Time.hpp>

namespace me
{
	class TimerSystem : public ISystem
	{
	public:

		void continuousUpdate(sf::Time timeElapsed) override;
		void fixedUpdate() override;

		TimerSystem() {}
	};
}

#endif // TIMER_SYSTEM_HPP