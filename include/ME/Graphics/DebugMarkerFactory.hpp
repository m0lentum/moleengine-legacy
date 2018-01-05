#ifndef DEBUG_MARKER_FACTORY_HPP
#define DEBUG_MARKER_FACTORY_HPP

#include <SFML/System/Vector2.hpp>

namespace me
{
	class GameObject;
	class Space;

	namespace DebugMarkerFactory
	{
		me::GameObject* makeCollisionLine(me::Space *space, const sf::Vector2f &point1, const sf::Vector2f &point2);
	}
}


#endif // DEBUG_MARKER_FACTORY_HPP