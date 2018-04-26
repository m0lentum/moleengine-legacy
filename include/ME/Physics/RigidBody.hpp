#ifndef PHYSICS_OBJECT_HPP
#define PHYSICS_OBJECT_HPP

#include "../GameObject.hpp"
#include <SFML/System/Vector2.hpp>
#include <memory>

namespace me
{
	class RigidBody
	{
	public: 

		bool isKinematic; // if true, object will not be affected by collisions

		sf::Vector2f velocity;
		float angularVelocity;

		float mass;
		float elasticity;
		float friction;
		float drag;
		float angularDrag;

		float gravityMultiplier; // Multiply Space gravity

	private:

		bool overridesGravity;
		sf::Vector2f gravityOverride; // Completely override Space gravity

	public:

		/// Apply a force to this object, meaning mass and (TODO) point of impact will be considered.
		void applyForce(const sf::Vector2f &force);
		/// Add a vector to the velocity of the object without regard to physical properties.
		inline void accelerate(const sf::Vector2f &acc) { velocity += acc; }

		void setGravityOverride(sf::Vector2f gravity);
		inline bool doesOverrideGravity() { return overridesGravity; }
		inline const sf::Vector2f getGravityOverride() { return gravityOverride; }
		inline void removeGravityOverride() { overridesGravity = false; }

		RigidBody();
		RigidBody(float mass, float elasticity, float friction, float drag, float angularDrag);
		RigidBody(bool isKinematic, float mass, float elasticity, float friction, float drag, float angularDrag);
		RigidBody(const RigidBody &copy);
		RigidBody(RigidBody&& move);
		RigidBody& operator=(RigidBody&& other);
		virtual ~RigidBody();
	};
}


#endif // PHYSICS_OBJECT_HPP