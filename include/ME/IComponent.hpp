#ifndef ICOMPONENT_HPP
#define ICOMPONENT_HPP

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Time.hpp>

namespace me
{
	/// Interface for things with update and draw loops.
	class IComponent
	{
	public:
		/// Update loop called as often as possible.
		virtual void continuousUpdate(const sf::Time &timeElapsed) = 0;
		/// Update loop called at a fixed time interval.
		virtual void fixedUpdate() = 0;
		/// Draw loop, called as often as possible.
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

		virtual ~IComponent() {}
	};
}


#endif //ICOMPONENT_HPP