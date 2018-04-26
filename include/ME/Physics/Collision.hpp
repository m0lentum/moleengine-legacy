#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <SFML/System/Vector2.hpp>

namespace me
{
	class GameObject;
	class RigidBody;

	struct Collision
	{
		GameObject *other;
		RigidBody *otherRB;
		sf::Vector2f penetration;
		sf::Vector2f manifold[2];
		sf::Vector2f impulse;

		Collision(GameObject *other, RigidBody *otherRB, const sf::Vector2f &penetration, const sf::Vector2f manifold[2]) :
			other(other),
			otherRB(otherRB),
			penetration(penetration)
		{
			this->manifold[0] = manifold[0];
			this->manifold[1] = manifold[1];
		}
	};
}

#endif // COLLISION_HPP