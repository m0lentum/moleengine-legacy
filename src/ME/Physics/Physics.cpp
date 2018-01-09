#include <Physics/Physics.hpp>
#include <Component.hpp>
#include <Space.hpp>
#include <Physics/RigidBody.hpp>
#include <vector>
#include <Physics/ICollider.hpp>
#include <Physics/ColliderCircle.hpp>
#include <Physics/ColliderRect.hpp>
#include <Physics/ColliderPolygon.hpp>
#include <Physics/PrimitiveQueries.hpp>
#include <Physics/Contact.hpp>
#include <Graphics/DebugMarkerFactory.hpp>

namespace me
{
	void Physics::fixedUpdate()
	{
		applyMovement();
		checkCollisions();
	}

	void Physics::applyMovement()
	{
		m_space->each<RigidBody>([&](Component<RigidBody>& comp)
		{
			RigidBody *rb = comp.getComponent();
			GameObject *parent = comp.getParent();

			if (!rb->isStatic)
			{
				if (!rb->isKinematic)
				{
					if (!rb->doesOverrideGravity())
						rb->accelerate(m_gravity * rb->gravityMultiplier);
					else
						rb->accelerate(rb->getGravityOverride());

					rb->velocity -= rb->drag * rb->velocity;
					rb->angularVelocity -= rb->angularDrag * rb->angularVelocity;
				}
				
				parent->move(rb->velocity);
				parent->rotate(rb->angularVelocity);
			}
		});
	}

	void Physics::checkCollisions()
	{
		std::vector<std::pair<GameObject*, ICollider*> > colliders;

		m_space->each<ColliderCircle>([&](Component<ColliderCircle> &comp)
		{
			colliders.push_back(std::pair<GameObject*, ICollider*>(comp.getParent(), comp.getComponent()));
		});
		m_space->each<ColliderRect>([&](Component<ColliderRect> &comp)
		{
			colliders.push_back(std::pair<GameObject*, ICollider*>(comp.getParent(), comp.getComponent()));
		});
		m_space->each<ColliderPolygon>([&](Component<ColliderPolygon> &comp)
		{
			colliders.push_back(std::pair<GameObject*, ICollider*>(comp.getParent(), comp.getComponent()));
		});

		using iter = std::vector<std::pair<GameObject*, ICollider*> >::iterator;
		for (iter i = colliders.begin(); i != colliders.end(); i++)
		{
			for (iter j = i + 1; j != colliders.end(); j++)
			{
				Contact info;
				info.obj1 = i->first;
				info.obj2 = j->first;
				PrimitiveQueries::checkCollision(*(i->second), *(j->second), info);


				if (info.areColliding)
				{
					info.obj1->move(info.penetration);

					//DebugMarkerFactory::makeLineSegment(m_space, info.obj1->getPosition(), info.obj2->getPosition());
					if (info.manifold[0].x != 0 || info.manifold[0].y != 0)
					{
						DebugMarkerFactory::makePoint(m_space, info.manifold[0]);
					}
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