#include <Physics/Physics.hpp>
#include <ComponentStorageUnit.hpp>
#include <Space.hpp>
#include <Physics/RigidBody.hpp>

namespace me
{
	void Physics::fixedUpdate()
	{
		applyMovement();
		checkCollisions();
	}

	void Physics::applyMovement()
	{
		m_space->each<RigidBody>([&](ComponentStorageUnit<RigidBody>& unit)
		{
			RigidBody *comp = unit.getComponent();
			GameObject *parent = unit.getParent();

			if (!comp->isStatic)
			{
				if (!comp->doesOverrideGravity())
					comp->accelerate(m_gravity * comp->gravityMultiplier);
				else
					comp->accelerate(comp->getGravityOverride());

				if (!comp->isKinematic)
				{
					comp->velocity -= comp->drag * comp->velocity;
					comp->angularVelocity -= comp->angularDrag * comp->angularVelocity;
				}
				
				parent->move(comp->velocity);
				parent->rotate(comp->angularVelocity);
			}
		});
	}

	void Physics::checkCollisions()
	{

	}


	Physics::Physics()
	{
	}

	Physics::Physics(const sf::Vector2f &gravity) :
		m_gravity(gravity)
	{
	}

	Physics::~Physics()
	{
	}
}