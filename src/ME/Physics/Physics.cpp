#include <Physics/Physics.hpp>
#include <ComponentStorageUnit.hpp>
#include <Space.hpp>
#include <Physics/RigidBody.hpp>
#include <vector>
#include <Physics/ICollider.hpp>
#include <Physics/ColliderCircle.hpp>
#include <Physics/ColliderRect.hpp>
#include <Physics/ColliderPolygon.hpp>
#include <Physics/CollisionChecker.hpp>
#include <Physics/CollisionInfo.hpp>

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
		std::vector<std::pair<GameObject*, ICollider*> > colliders;

		m_space->each<ColliderCircle>([&](ComponentStorageUnit<ColliderCircle> &unit)
		{
			colliders.push_back(std::pair<GameObject*, ICollider*>(unit.getParent(), unit.getComponent()));
		});
		m_space->each<ColliderRect>([&](ComponentStorageUnit<ColliderRect> &unit)
		{
			colliders.push_back(std::pair<GameObject*, ICollider*>(unit.getParent(), unit.getComponent()));
		});
		m_space->each<ColliderPolygon>([&](ComponentStorageUnit<ColliderPolygon> &unit)
		{
			colliders.push_back(std::pair<GameObject*, ICollider*>(unit.getParent(), unit.getComponent()));
		});

		using iter = std::vector<std::pair<GameObject*, ICollider*> >::iterator;
		for (iter i = colliders.begin(); i != colliders.end(); i++)
		{
			for (iter j = i + 1; j != colliders.end(); j++)
			{
				CollisionInfo info;
				info.obj1 = i->first;
				info.obj2 = j->first;
				CollisionChecker::checkCollision(*(i->second), *(j->second), info);


				if (info.areColliding)
				{
					info.obj1->move(info.penetration);
				}
			}
		}
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