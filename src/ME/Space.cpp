#include <Space.hpp>
#include <Physics/CollisionChecker.hpp>
//#include <iostream>

namespace me
{
	void Space::continuousUpdate(sf::Time timeElapsed)
	{
		for (auto &cont : m_controllers)
		{
			cont->continuousUpdate(timeElapsed);
		}
	}

	void Space::fixedUpdate()
	{
		for (auto &cont : m_controllers)
		{
			cont->fixedUpdate();
		}
	}

	void Space::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		for (auto &cont : m_controllers)
		{
			cont->draw(target, states);
		}
	}


	void Space::addController(IController *controller)
	{
		m_controllers.push_back(std::unique_ptr<IController>(controller));
	}

	GameObject * Space::createObject()
	{
		if (m_objects.size() < m_objects.capacity())
		{
			m_objects.push_back(GameObject(this));
			return &(m_objects.back());
		}
		else
		{
			std::cerr << "Error: Space is full" << std::endl;
			return NULL;
		}
	}


	Space::Space(std::size_t maxObjects) :
		m_maxObjects(maxObjects)
	{
		m_objects.reserve(maxObjects); // Reserve room for all objects now so reallocations won't happen later
	}

	Space::~Space()
	{
	}
}