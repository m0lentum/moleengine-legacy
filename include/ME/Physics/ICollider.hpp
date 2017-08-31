#ifndef ICOLLIDER_HPP
#define ICOLLIDER_HPP

#include <SFML/Graphics/Transform.hpp>
#include "CollisionInfo.hpp"
#include <memory>
#include <SFML/Graphics/Color.hpp>

namespace me
{
	class ColliderCircle;
	class ColliderRect;
	class ColliderPolygon;

	class PhysicsObject;

	class Graphic;

	/// Interface for different collider shapes used in collision detection.
	class ICollider
	{
	protected:
		me::PhysicsObject *m_parent;

	public:
		inline void setParent(me::PhysicsObject *obj) { m_parent = obj; }

		/// This will resolve the type of the first object similarly to the Visitor pattern.
		/// One of the other collider's checkCollision methods will be called accordingly to the result.
		virtual void findTypeAndCheckCollision(const ICollider &other, CollisionInfo &info) const = 0;

		// Every collider type must implement collisions against every other type.
		// These functions will be called by the other object involved in the collision.
		// Results will be written into the given CollisionInfo object.
		virtual void checkCollision(const ColliderCircle &other, CollisionInfo &info) const = 0;
		virtual void checkCollision(const ColliderRect &other, CollisionInfo &info) const = 0;
		virtual void checkCollision(const ColliderPolygon &other, CollisionInfo &info) const = 0;
		
		/// Generate a Graphic from the shape of the collider.
		virtual std::shared_ptr<Graphic> generateGraphic(const sf::Color &color = sf::Color::White) const = 0;

		/// Virtual clone method to allow copying from a base class pointer
		virtual ICollider* clone() const = 0;

		virtual ~ICollider() {}
	};
}

#endif // ICOLLIDER_HPP