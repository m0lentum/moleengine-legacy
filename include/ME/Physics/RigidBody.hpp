#ifndef PHYSICS_OBJECT_HPP
#define PHYSICS_OBJECT_HPP

#include "../GameObject.hpp"
#include <SFML/System/Vector2.hpp>
#include <memory>
#include "PhysicsMaterial.hpp"

namespace me
{
	/// Inverted mass is needed often so we precompute it into a data structure
	struct MassInfo
	{
		float actual;
		float inverted;

		MassInfo& operator=(float mass)
		{
			actual = mass;
			inverted = 1 / mass;
			return *this;
		}
	};

	class RigidBody
	{
	public:

		bool isKinematic; // if true, object can only be moved by directly changing its velocity or position

		sf::Vector2f velocity;
		float angularVelocity;

		MassInfo mass;
		MassInfo momentOfInertia;

        PhysicsMaterial material;

		float gravityMultiplier; // Multiply Space gravity


        static const PhysicsMaterial defaultMaterial;

	private:

		bool overridesGravity;
		sf::Vector2f gravityOverride; // Completely override Space gravity

	public:

		/// Apply a force at the center of mass (not affecting rotation)
		void applyImpulse(const sf::Vector2f &force);
		/// Apply a force at an offset from the center of mass
		void applyImpulse(const sf::Vector2f &force, const sf::Vector2f &offset);
		/// Add a vector to the velocity of the object without regard to physical properties.
		inline void accelerate(const sf::Vector2f &acc) { velocity += acc; }


		sf::Vector2f getPointVelocity(const sf::Vector2f &offset) const;

		void setGravityOverride(sf::Vector2f gravity);
		inline bool doesOverrideGravity() { return overridesGravity; }
		inline const sf::Vector2f getGravityOverride() { return gravityOverride; }
		inline void removeGravityOverride() { overridesGravity = false; }

		RigidBody();
		RigidBody(float mass, float momentOfInertia, PhysicsMaterial material);
		RigidBody(bool isKinematic, float mass, float momentOfInertia, PhysicsMaterial material);
		RigidBody(const RigidBody &copy);
		RigidBody(RigidBody&& move);
		RigidBody& operator=(RigidBody&& other);
		virtual ~RigidBody();
	};
}


#endif // PHYSICS_OBJECT_HPP