#include <Physics/RigidBody.hpp>
#include <iostream>
#include <Space.hpp>
#include <Physics/VectorMath.hpp>

namespace me
{
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
		RigidBody(false, 1.0f, 300.0f, 0.75f, 0.05f, 0.002f, 0.001f)
	{
	}

	RigidBody::RigidBody(float mass, float momentOfInertia, float elasticity, float friction, float drag, float angularDrag) :
		RigidBody(false, mass, momentOfInertia, elasticity, friction, drag, angularDrag)
	{
	}

	RigidBody::RigidBody(bool isKinematic, float mass, float momentOfInertia, float elasticity, float friction, float drag, float angularDrag) :
		isKinematic(isKinematic),
		angularVelocity(0),
		elasticity(elasticity),
		friction(friction),
		drag(drag),
		angularDrag(angularDrag),
		gravityMultiplier(1),
		overridesGravity(false)
	{
		this->mass = mass;
		this->momentOfInertia = momentOfInertia;
	}

	RigidBody::RigidBody(const RigidBody &copy) :
		isKinematic(copy.isKinematic),
		velocity(copy.velocity),
		angularVelocity(copy.angularVelocity),
		mass(copy.mass),
		momentOfInertia(copy.momentOfInertia),
		elasticity(copy.elasticity),
		friction(copy.friction),
		drag(copy.drag),
		angularDrag(copy.angularDrag),
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
		elasticity(std::move(move.elasticity)),
		friction(std::move(move.friction)),
		drag(std::move(move.drag)),
		angularDrag(std::move(move.angularDrag)),
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
			elasticity = std::move(other.elasticity);
			friction = std::move(other.friction);
			drag = std::move(other.drag);
			angularDrag = std::move(other.angularDrag);
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