#include <Space.hpp>
//#include <iostream>

namespace me
{
	void Space::continuousUpdate(sf::Time timeElapsed)
	{
		for (const auto &obj : m_objects)
			obj.second->continuousUpdate(timeElapsed);
	}
	
	void Space::fixedUpdate()
	{
		// Remove all objects marked for destruction
		while (!m_toDestroy.empty())
		{
			m_objects.erase(m_toDestroy.front());
			m_toDestroy.pop();
		}
		// Update the rest of the objects
		for (const auto &obj : m_objects)
			obj.second->fixedUpdate();
	}
	
	void Space::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		for (const auto &obj : m_objects)
			obj.second->draw(target, states);
	}

	void Space::addObject(std::shared_ptr<GameObject> object)
	{
		m_objects.emplace(object.get(), object);
		object->registerSpace(this);
	}

	void Space::removeObject(GameObject * object)
	{
		m_toDestroy.push(object);
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