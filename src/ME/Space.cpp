#include <Space.hpp>
//#include <iostream>

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
		// TODO: physics calculations
	}
	
	void Space::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		doDraw<GameObject>(m_objects, target, states);
		doDraw<PhysicsObject>(m_physicsObjects, target, states);
	}

	void Space::addObject(std::shared_ptr<GameObject> object)
	{
		m_objects.emplace(object.get(), object);
		object->registerSpace(this);
	}

	void Space::addPhysicsObject(std::shared_ptr<PhysicsObject> object)
	{
		m_physicsObjects.emplace(object.get(), object);
		object->registerSpace(this);
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