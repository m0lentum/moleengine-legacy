#ifndef SPACE_HPP
#define SPACE_HPP

#include <unordered_map>
#include <queue>
#include "IController.hpp"
#include "GameObject.hpp"
#include "IController.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace me
{
	class IComponent;

	/// A container class for GameObjects. Used to group objects and share them between game states.
	class Space
	{
	private:
		/// A set of all non-physics objects. These objects are owned by the Space and should not have references stored elsewhere.
		std::unordered_map<std::string, std::unique_ptr<GameObject> > m_objects;
		/// Store the ids of the objects we need to destroy in a queue for 
		/// deletion at the start of the next update loop
		std::queue<std::string> m_toDestroy;
		/// Controllers that manage the Components present in GameObjects
		std::unordered_map<std::string, std::unique_ptr<IController> > m_controllers;

	public:
		/// continuousUpdate all contained objects
		void continuousUpdate(sf::Time timeElapsed);
		/// fixedUpdate all contained objects
		void fixedUpdate();
		/// draw all contained objects
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;



		void addObject(GameObject *object);
		void removeObject(const std::string &id);
		/// Called by GameObject if a component gets added to it while it's already in a Space
		void addComponent(IComponent *component);

		void addController(IController *controller);
		void removeController(const std::string &type);


		Space();
		Space(const Space &copy);
		~Space();
	};
}


#endif //SPACE_HPP