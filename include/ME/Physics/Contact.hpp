#ifndef COLLISION_INFO_HPP
#define COLLISION_INFO_HPP

#include <SFML/System/Vector2.hpp>
#include <vector>

namespace me
{
	class GameObject;
	class RigidBody;

	/// Struct containing information about a collision.
	struct Contact
	{
		GameObject *obj1;
		GameObject *obj2;
		RigidBody *rb1;
		RigidBody *rb2;

		bool areColliding = false;

		/// The contact point manifold containing up to 2 points in global space. The points are always on the surface of obj1.
		sf::Vector2f manifold[2];
		/// The direction and depth of penetration if the objects are colliding (direction towards obj1).
		sf::Vector2f penetration;
	};
}


#endif // COLLISION_INFO_HPP