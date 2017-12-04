#ifndef SPACE_HPP
#define SPACE_HPP

#include <vector>
#include <queue>
#include <memory>
#include <initializer_list>
#include "GameObject.hpp"
#include "IController.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace me
{
	/// A container class for GameObjects. Used to group objects and share them between game states.
	class Space
	{
	private:

		std::vector<std::unique_ptr<IController> > m_controllers;

		std::vector<GameObject> m_objects;
		std::queue<unsigned int> m_toDestroy;

	public:

		void continuousUpdate(sf::Time timeElapsed);
		void fixedUpdate();
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;


		void addController(IController *controller);

		GameObject * createObject();


		Space();
		Space(const Space &copy);
		~Space();
	};
}


#endif //SPACE_HPP