#include <Physics/RigidBody.hpp>
#include <iostream>
#include <Space.hpp>

namespace me
{
	void RigidBody::applyForce(const sf::Vector2f &force)
	{
		velocity += (force / mass);
	}

	void RigidBody::setGravityOverride(sf::Vector2f gravity)
	{
		overridesGravity = true;
		gravityOverride = gravity;
	}



	RigidBody::RigidBody() :
		RigidBody(false, false, 1.0f, 0.75f, 0.05f, 0.02f, 0.01f)
	{
	}

	RigidBody::RigidBody(float mass, float elasticity, float friction, float drag, float angularDrag) :
		RigidBody(false, false, mass, elasticity, friction, drag, angularDrag)
	{
	}

	RigidBody::RigidBody(bool isStatic, bool isKinematic, float mass, float elasticity, float friction, float drag, float angularDrag) :
		isStatic(isStatic),
		isKinematic(isKinematic),
		angularVelocity(0),
		mass(mass),
		elasticity(elasticity),
		friction(friction),
		drag(drag),
		angularDrag(angularDrag),
		gravityMultiplier(1),
		overridesGravity(false)
	{
	}

	RigidBody::RigidBody(const RigidBody &copy) :
		isStatic(copy.isStatic),
		isKinematic(copy.isKinematic),
		velocity(copy.velocity),
		angularVelocity(copy.angularVelocity),
		mass(copy.mass),
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
		isStatic(std::move(move.isStatic)),
		isKinematic(std::move(move.isKinematic)),
		velocity(std::move(move.velocity)),
		angularVelocity(std::move(move.angularVelocity)),
		mass(std::move(move.mass)),
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
			isStatic = std::move(other.isStatic);
			isKinematic = std::move(other.isKinematic);
			velocity = std::move(other.velocity);
			angularVelocity = std::move(other.angularVelocity);
			mass = std::move(other.mass);
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