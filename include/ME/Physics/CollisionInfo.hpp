#ifndef COLLISION_INFO_HPP
#define COLLISION_INFO_HPP

#include <SFML/System/Vector2.hpp>
#include <vector>

namespace me
{
	class PhysicsObject;

	/// Struct containing information about a collision.
	struct CollisionInfo
	{
		PhysicsObject *obj1;
		PhysicsObject *obj2;

		bool areColliding = false;

		/// The point on object 1 (in local space) where the collision force will be applied
		sf::Vector2f point1;
		/// Same as point1 but for object 2
		sf::Vector2f point2;
		/// The direction and depth of penetration if the objects are colliding (direction towards obj1).
		sf::Vector2f penetration;
	};
}


#endif // COLLISION_INFO_HPP