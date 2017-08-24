#include <Input/Keyboard.hpp>
#include <iostream>

namespace me
{
	std::unordered_map<sf::Keyboard::Key, bool> Keyboard::m_trackedKeys = std::unordered_map<Key, bool>();

	void Keyboard::trackKey(Key key)
	{
		Keyboard::m_trackedKeys[key] = false;
	}

	bool Keyboard::wasKeyPressed(Key key)
	{
		if (Keyboard::m_trackedKeys.count(key) > 0)
		{
			return Keyboard::m_trackedKeys[key];
		}
		else
		{
			// the specified key isn't being tracked
			std::cerr << "Keyboard::wasKeyPressed called for an untracked key: " << key << std::endl;
			return false;
		}
	}

	bool Keyboard::isKeyJustPressed(Key key)
	{
		return isKeyPressed(key) && !wasKeyPressed(key);
	}

	bool Keyboard::isKeyJustReleased(Key key)
	{
		return !isKeyPressed(key) && wasKeyPressed(key);
	}

	void Keyboard::updateTracked()
	{
		for (auto val : Keyboard::m_trackedKeys)
		{
			Keyboard::m_trackedKeys[val.first] = isKeyPressed(val.first);
		}
	}
}