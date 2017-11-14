#include <Space.hpp>
#include <Physics/CollisionChecker.hpp>
//#include <iostream>

namespace me
{
	void Space::continuousUpdate(sf::Time timeElapsed)
	{
		for (const auto &obj : m_objects) obj->continuousUpdate(timeElapsed);
		for (const auto &pObj : m_physicsObjects) pObj->continuousUpdate(timeElapsed);
	}
	

	void Space::fixedUpdate()
	{
		// Remove all objects marked for destruction
		while (!m_oToDestroy.empty())
		{
			GameObject *obj = m_oToDestroy.front();
			m_objects.erase(obj);
			m_oToDestroy.pop();

			delete obj;
		}
		// Update the rest of the objects
		for (const auto &obj : m_objects)
			obj->fixedUpdate();


		// Same for physics objects
		while (!m_pToDestroy.empty())
		{
			PhysicsObject *obj = m_pToDestroy.front();
			m_physicsObjects.erase(obj);
			m_pToDestroy.pop();

			delete obj;
		}
		for (const auto &obj : m_physicsObjects)
			obj->fixedUpdate();

		handleCollisions();
	}
	

	void Space::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		for (const auto &obj : m_objects) obj->draw(target, states);
		for (const auto &pObj : m_physicsObjects) pObj->draw(target, states);
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
				info.obj1 = *i;
				info.obj2 = *j;
				CollisionChecker::checkCollision((*i)->getCollider(), (*j)->getCollider(), info);

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
		// TODO: physics
	}



	void Space::addObject(GameObject * object)
	{
		m_objects.emplace(object);
	}

	void Space::addObject(PhysicsObject * object)
	{
		m_physicsObjects.emplace(object);
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