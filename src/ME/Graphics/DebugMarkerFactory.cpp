#include <Graphics/DebugMarkerFactory.hpp>
#include <GameObject.hpp>
#include <Graphics/Graphic.hpp>
#include <Space.hpp>
#include <SFML/Graphics/Color.hpp>
#include <Utility/DelayedEventComponent.hpp>
#include <iostream>

namespace me
{
	namespace DebugMarkerFactory
	{
		me::GameObject* makeCollisionLine(Space *space, const sf::Vector2f &point1, const sf::Vector2f &point2)
		{
			me::GameObject *obj = space->createObject();

			if (obj)
			{
				obj->addComponent<me::Graphic>(me::Graphic::makeLineSegment(point1, point2, 5.0f, sf::Color::Red));
				obj->addComponent<me::DelayedEvent>(2, [obj]() {
					obj->destroy();
				});
				

				return obj;
			}

			return NULL;
		}
	}
}