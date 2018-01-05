#ifndef UPDATE_LOOP_COMPONENT_HPP
#define UPDATE_LOOP_COMPONENT_HPP

#include <functional>
#include <SFML/System/Time.hpp>

namespace me
{
	class FixedUpdateLoop
	{
	private:

		std::function<void()> m_function;

	public:

		inline void update() { m_function(); }

		FixedUpdateLoop(std::function<void()> function) :
			m_function(function)
		{
		}

		~FixedUpdateLoop() {}
	};

	class ContinuousUpdateLoop
	{
	private:

		std::function<void(sf::Time)> m_function;

	public:

		inline void update(sf::Time timeElapsed) { m_function(timeElapsed); }

		ContinuousUpdateLoop(std::function<void(sf::Time)> function) :
			m_function(function)
		{
		}

		~ContinuousUpdateLoop() {}
	};
}

#endif // UPDATE_LOOP_COMPONENT_HPP