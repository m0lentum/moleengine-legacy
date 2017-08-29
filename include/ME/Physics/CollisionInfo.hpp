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
		const PhysicsObject *obj1;
		const PhysicsObject *obj2;

		bool areColliding = false;

		/// The point in global space where the force of the collision will be applied to objects.
		sf::Vector2f point;
		sf::Vector2f penetration;
	};
}


#endif // COLLISION_INFO_HPP