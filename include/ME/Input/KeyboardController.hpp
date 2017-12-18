#ifndef KEYBOARD_CONTROLLER_HPP
#define KEYBOARD_CONTROLLER_HPP

#include <functional>
#include <SFML/Window/Event.hpp>

namespace me
{
	struct KeyboardController
	{
		std::function<void(const sf::Event::KeyEvent&)> onKeyPressed;
		std::function<void(const sf::Event::KeyEvent&)> onKeyReleased;
	};
}

#endif // KEYBOARD_CONTROLLER_HPP