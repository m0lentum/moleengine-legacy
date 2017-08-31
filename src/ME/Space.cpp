#include <Space.hpp>
#include <Physics/CollisionChecker.hpp>
#include <iostream>

namespace me
{
	void Space::continuousUpdate(sf::Time timeElapsed)
	{
		doContinuousUpdate<GameObject>(m_objects, timeElapsed);
		doContinuousUpdate<PhysicsObject>(m_physicsObjects, timeElapsed);
	}
	
	void Space::fixedUpdate()
	{
		doFixedUpdate<GameObject>(m_objects, m_oToDestroy);
		doFixedUpdate<PhysicsObject>(m_physicsObjects, m_pToDestroy);
		
		handleCollisions();
	}
	
	void Space::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		doDraw<GameObject>(m_objects, target, states);
		doDraw<PhysicsObject>(m_physicsObjects, target, states);
	}

	void Space::handleCollisions()
	{
		// TODO: add optimization through spatial partitioning

		for (auto i = m_physicsObjects.begin(); i != m_physicsObjects.end(); i++)
		{
			auto j = i;
			j++;
			for (; j != m_physicsObjects.end(); j++)
			{
				CollisionInfo info;
				info.obj1 = i->first;
				info.obj2 = j->first;
				CollisionChecker::checkCollision(i->second->getCollider(), j->second->getCollider(), info);

				if (info.areColliding)
				{
					// TODO: onCollision events for objects
					resolveCollision(info);
				}
			}
		}
	}

	void Space::resolveCollision(const CollisionInfo &info)
	{
		info.obj1->move(info.penetration);
	}


	void Space::removeObject(GameObject * object)
	{
		m_oToDestroy.push(object);
	}

	void Space::removeObject(PhysicsObject * object)
	{
		m_pToDestroy.push(object);
	}



	Space::Space()
	{
	}

	Space::Space(const Space &copy) :
		m_objects(copy.m_objects)
	{
	}

	Space::~Space()
	{
	}
}