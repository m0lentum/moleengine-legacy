#ifndef MOUSE_CONTROLLER_HPP
#define MOUSE_CONTROLLER_HPP

#include <functional>
#include <SFML/Window/Event.hpp>

namespace me
{
	struct MouseController
	{
		std::function<void(const sf::Event::MouseMoveEvent&)> onMouseMoved;
		std::function<void(const sf::Event::MouseMoveEvent&)> onMouseEnter;
		std::function<void(const sf::Event::MouseMoveEvent&)> onMouseExit;
		std::function<void(const sf::Event::MouseButtonEvent&)> onMouseButtonPressed;
		std::function<void(const sf::Event::MouseButtonEvent&)> onMouseButtonReleased;
		std::function<void(const sf::Event::MouseWheelScrollEvent&)> onMouseWheelScrolled;
	};
}

#endif // MOUSE_CONTROLLER_HPP