#include <Graphics/DebugMarkerFactory.hpp>
#include <GameObject.hpp>
#include <Graphics/Graphic.hpp>
#include <Space.hpp>
#include <SFML/Graphics/Color.hpp>
#include <Utility/DelayedEventComponent.hpp>
#include <iostream>

namespace me
{
	GameObject* DebugMarkerFactory::makeLineSegment(Space *space, const sf::Vector2f &point1, const sf::Vector2f &point2)
	{
		GameObject *obj = space->createObject();

		if (obj)
		{
			obj->addComponent<Graphic>(Graphic::makeLineSegment(point1, point2, 5.0f, sf::Color::Red));
			obj->addComponent<DelayedEvent>(2, [obj]() {
				obj->destroy();
			});
				

			return obj;
		}

		return NULL;
	}

	GameObject* DebugMarkerFactory::makePoint(Space *space, const sf::Vector2f &position)
	{
		GameObject *obj = space->createObject();

		if (obj)
		{
			obj->addComponent<Graphic>(Graphic::makeCircle(5.0f, 8, sf::Color::Magenta));
			obj->addComponent<DelayedEvent>(2, [obj]() {
				obj->destroy();
			});

			obj->setPosition(position);

			return obj;
		}

		return NULL;
	}
}