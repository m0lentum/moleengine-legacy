#include <Space.hpp>
//#include <iostream>

namespace me
{
	void Space::continuousUpdate(sf::Time timeElapsed)
	{
		for (const auto &obj : m_objects)
			obj->continuousUpdate(timeElapsed);
	}
	
	void Space::fixedUpdate()
	{
		for (const auto &obj : m_objects)
			obj->fixedUpdate();
	}
	
	void Space::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		for (const auto &obj : m_objects)
			target.draw(*obj, states);
	}

	void Space::addObject(GameObject *object)
	{
		m_objects.push_back(std::shared_ptr<GameObject>(object));
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