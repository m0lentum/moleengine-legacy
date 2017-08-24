#ifndef ME_KEYBOARD_HPP
#define ME_KEYBOARD_HPP

#include <SFML/Window/Keyboard.hpp>
#include <unordered_map>

namespace me
{
	/// Class that adds tracking of previously held keys to sf::Keyboard.
	/// You should only handle previous inputs in the fixedUpdate loop,
	/// since that's when they are updated.
	class Keyboard : public sf::Keyboard
	{
	private:
		static std::unordered_map<Key, bool> m_trackedKeys;

	public:
		/// Add a key to the list of tracked keys. (only track ones you need instead of all of them)
		static void trackKey(Key key);
		/// Whether or not a key was pressed the previous frame
		static bool wasKeyPressed(Key key);
		/// Whether or not a key is pressed and it's the first frame of it being pressed 
		static bool isKeyJustPressed(Key key);
		/// Whether or not a key is released and it's the first frame of it being released 
		static bool isKeyJustReleased(Key key);

		/// Updates the states of the tracked keys
		static void updateTracked();
	};
}



#endif //ME_KEYBOARD_HPP