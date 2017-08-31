#ifndef SPACE_HPP
#define SPACE_HPP

#include <unordered_map>
#include <memory>
#include <queue>
#include <type_traits>
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
		void continuousUpdate(sf::Time timeElapsed);
		/// fixedUpdate all contained objects
		void fixedUpdate();
		/// draw all contained objects
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	private:
		/// Check for collisions and resolve them
		void handleCollisions();
		/// Resolves penetration and applies appropriate forces to the objects.
		void resolveCollision(const CollisionInfo &info);

	public:

		// TODO: tagging system to filter objects

		Space();
		Space(const Space &copy);
		~Space();

		// messy code, but this allows using the same method name to add both PhysicsObjects and GameObjects, 
		// and thus eliminates the possibility of adding an object to the wrong container by accident.

		/// add a GameObject
		template<typename T, typename std::enable_if<std::is_base_of<GameObject, T>::value && !std::is_base_of<PhysicsObject, T>::value>::type* = nullptr> 
		void addObject(std::shared_ptr<T> object)
		{
			m_objects.emplace(object.get(), object);
			object->registerSpace(this);
		}
		/// add a PhysicsObject
		template<typename T, typename std::enable_if<std::is_base_of<PhysicsObject, T>::value>::type* = nullptr>
		void addObject(std::shared_ptr<T> object)
		{
			m_physicsObjects.emplace(object.get(), object);
			object->registerSpace(this);
		}

		void removeObject(GameObject * object);
		void removeObject(PhysicsObject * object);
		
	private:
		// Helper functions to avoid copy-pasting code.
		// These won't be used outside the class so I'm too lazy to put them in an .inl file.

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