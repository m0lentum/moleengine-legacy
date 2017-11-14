#ifndef SPACE_HPP
#define SPACE_HPP

#include <unordered_set>
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
		/// A set of all non-physics objects. These objects are owned by the Space and should not have references stored elsewhere.
		std::unordered_set<GameObject*> m_objects;
		/// Store physics objects in a separate container so we can access their special properties
		std::unordered_set<PhysicsObject*> m_physicsObjects;
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

		void addObject(GameObject * object);
		void addObject(PhysicsObject * object);

		void removeObject(GameObject * object);
		void removeObject(PhysicsObject * object);

		// TODO: tagging system to filter objects

		Space();
		Space(const Space &copy);
		~Space();
	};
}


#endif //SPACE_HPP