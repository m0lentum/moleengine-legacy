#ifndef SPACE_HPP
#define SPACE_HPP

#include <unordered_map>
#include <queue>
#include "GameObject.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace me
{
	class IComponent;

	/// A container class for GameObjects. Used to group objects and share them between game states.
	class Space
	{
	private:


	public:
		/// continuousUpdate all contained objects
		void continuousUpdate(sf::Time timeElapsed);
		/// fixedUpdate all contained objects
		void fixedUpdate();
		/// draw all contained objects
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;



		Space();
		Space(const Space &copy);
		~Space();
	};
}


#endif //SPACE_HPP