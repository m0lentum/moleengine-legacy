#ifndef SPACE_HPP
#define SPACE_HPP

#include <unordered_map>
#include <memory>
#include <queue>
#include "GameObject.hpp"
#include "Physics/PhysicsObject.hpp"
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
		/// Store physics objects in a separate container so we can access their special properties
		std::unordered_map<PhysicsObject*, std::shared_ptr<PhysicsObject>> m_physicsObjects;
		/// Store the objects we need to destroy in a queue for 
		/// deletion at the start of the next update loop
		std::queue<GameObject*> m_oToDestroy;
		std::queue<PhysicsObject*> m_pToDestroy;

	public:
		/// continuousUpdate all contained objects
		virtual void continuousUpdate(sf::Time timeElapsed);
		/// fixedUpdate all contained objects
		virtual void fixedUpdate();
		/// draw all contained objects
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

		void addObject(std::shared_ptr<GameObject> object);
		void addPhysicsObject(std::shared_ptr<PhysicsObject> object);
		void removeObject(GameObject * object);
		void removeObject(PhysicsObject * object);

		// TODO: tagging system to filter objects

		Space();
		Space(const Space &copy);
		~Space();



	private:
		// Helper functions to avoid copy-pasting code in the actual update loops.
		// These are not really important for anyone reading the code so I'm too lazy to put these in an .inl file.

		template<class T>
		void doContinuousUpdate(std::unordered_map<T*, std::shared_ptr<T>> &objects, sf::Time timeElapsed)
		{
			for (const auto &obj : objects) obj.second->continuousUpdate(timeElapsed);
		}

		template<class T>
		void doFixedUpdate(std::unordered_map<T*, std::shared_ptr<T>> &objects, std::queue<T*> &toDestroy)
		{
			// Remove all objects marked for destruction
			while (!toDestroy.empty())
			{
				objects.erase(toDestroy.front());
				toDestroy.pop();
			}
			// Update the rest of the objects
			for (const auto &obj : objects)
				obj.second->fixedUpdate();
		}

		template<class T>
		void doDraw(const std::unordered_map<T*, std::shared_ptr<T>> &objects, sf::RenderTarget &target, sf::RenderStates states) const
		{
			for (const auto &obj : objects)
				obj.second->draw(target, states);
		}
	};
}


#endif //SPACE_HPP