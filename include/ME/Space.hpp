#ifndef SPACE_HPP
#define SPACE_HPP

#include <unordered_map>
#include <memory>
#include <queue>
#include "GameObject.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace me
{
	/// A container class for GameObjects. Used to group objects and share them between game states.
	class Space
	{
	private:
		/// Store all objects in a map with a pointer to themselves as key.
		/// This allows a simple way for the object to delete itself,
		/// while still using shared_ptr for memory management.
		std::unordered_map<GameObject*, std::shared_ptr<GameObject>> m_objects;
		/// Store the objects we need to destroy in a queue for 
		/// deletion at the start of the next update loop
		std::queue<GameObject*> m_toDestroy;

	public:
		/// continuousUpdate all contained objects
		virtual void continuousUpdate(sf::Time timeElapsed);
		/// fixedUpdate all contained objects
		virtual void fixedUpdate();
		/// draw all contained objects
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

		/// Iterator to the beginning of the list
		inline std::unordered_map<GameObject*, std::shared_ptr<GameObject>>::iterator begin() { return m_objects.begin(); }
		/// Iterator to the end of the list
		inline std::unordered_map<GameObject*, std::shared_ptr<GameObject>>::iterator end() { return m_objects.end(); }

		void addObject(std::shared_ptr<GameObject> object);
		void removeObject(GameObject * object);

		// TODO: tagging system to filter objects

		Space();
		Space(const Space &copy);
		~Space();
	};
}


#endif //SPACE_HPP