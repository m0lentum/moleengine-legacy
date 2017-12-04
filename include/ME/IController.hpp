#ifndef ICONTROLLER_HPP
#define ICONTROLLER_HPP

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace me
{
	/// An interface for classes that control various Components.
	class IController
	{
	public:

		// Update loops are optional.
		virtual void continuousUpdate(sf::Time timeElapsed) {}
		virtual void fixedUpdate() {}
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const {}

		virtual ~IController() {}
	};
}

#endif // ICONTROLLER_HPP