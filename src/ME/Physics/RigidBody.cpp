#include <Physics/RigidBody.hpp>
#include <iostream>
#include <Space.hpp>
#include <Physics/VectorMath.hpp>

namespace me
{
    const PhysicsMaterial RigidBody::defaultMaterial = PhysicsMaterial(0.75f, 0.05f, 0.002f, 0.001f, 0.1f);

    
	void RigidBody::applyImpulse(const sf::Vector2f &force)
	{
		velocity += force * mass.inverted;
	}

	void RigidBody::applyImpulse(const sf::Vector2f &force, const sf::Vector2f &offset)
	{
		velocity += force * mass.inverted;
		angularVelocity += VectorMath::cross2D(offset, force) * momentOfInertia.inverted;
	}

	void RigidBody::setGravityOverride(sf::Vector2f gravity)
	{
		overridesGravity = true;
		gravityOverride = gravity;
	}

	sf::Vector2f RigidBody::getPointVelocity(const sf::Vector2f &offset) const
	{
		return velocity + VectorMath::cross2D(angularVelocity, offset);
	}



	RigidBody::RigidBody() :
		RigidBody(false, 1.0f, defaultMaterial)
	{
	}

	RigidBody::RigidBody(float mass, float momentOfInertia, PhysicsMaterial material) :
		RigidBody(false, mass, momentOfInertia, material)
	{
	}

	RigidBody::RigidBody(bool isKinematic, float mass, float momentOfInertia, PhysicsMaterial material) :
		isKinematic(isKinematic),
		angularVelocity(0),
        material(material),
		gravityMultiplier(1),
		overridesGravity(false)
	{
        // assignment like this automatically calculates and stores the inverses
		this->mass = mass;
		this->momentOfInertia = momentOfInertia;
	}

	RigidBody::RigidBody(const RigidBody &copy) :
		isKinematic(copy.isKinematic),
		velocity(copy.velocity),
		angularVelocity(copy.angularVelocity),
		mass(copy.mass),
        momentOfInertia(copy.momentOfInertia),
        material(copy.material),
		gravityOverride(copy.gravityOverride),
		gravityMultiplier(copy.gravityMultiplier),
		overridesGravity(copy.overridesGravity)
	{
	}

	RigidBody::RigidBody(RigidBody&& move) :
		isKinematic(std::move(move.isKinematic)),
		velocity(std::move(move.velocity)),
		angularVelocity(std::move(move.angularVelocity)),
		mass(std::move(move.mass)),
		momentOfInertia(std::move(move.momentOfInertia)),
        material(std::move(move.material)),
		gravityOverride(std::move(move.gravityOverride)),
		gravityMultiplier(std::move(move.gravityMultiplier)),
		overridesGravity(std::move(move.overridesGravity))
	{
	}

	RigidBody& RigidBody::operator=(RigidBody&& other)
	{
		if (this != &other)
		{
			isKinematic = std::move(other.isKinematic);
			velocity = std::move(other.velocity);
			angularVelocity = std::move(other.angularVelocity);
			mass = std::move(other.mass);
			momentOfInertia = std::move(other.momentOfInertia);
            material = std::move(other.material);
			gravityOverride = std::move(other.gravityOverride);
			gravityMultiplier = std::move(other.gravityMultiplier);
			overridesGravity = std::move(other.overridesGravity);
		}

		return *this;
	}

	RigidBody::~RigidBody()
	{
	}
}