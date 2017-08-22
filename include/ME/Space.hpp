#ifndef SPACE_HPP
#define SPACE_HPP

#include <vector>
#include <memory>
#include "GameObject.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace me
{
	/// A container class for GameObjects. Used to group objects and share them between game states.
	class Space
	{
	private:
		std::vector<std::shared_ptr<GameObject>> m_objects;

	public:
		/// continuousUpdate all contained objects
		virtual void continuousUpdate(sf::Time timeElapsed);
		/// fixedUpdate all contained objects
		virtual void fixedUpdate();
		/// draw all contained objects
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

		/// Iterator to the beginning of the list
		inline std::vector<std::shared_ptr<GameObject>>::iterator begin() { return m_objects.begin(); }
		/// Iterator to the end of the list
		inline std::vector<std::shared_ptr<GameObject>>::iterator end() { return m_objects.end(); }

		void addObject(GameObject *object);

		// TODO: tagging system to filter objects

		Space();
		Space(const Space &copy);
		~Space();
	};
}


#endif //SPACE_HPP