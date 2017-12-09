#include <Space.hpp>
#include <Physics/CollisionChecker.hpp>
#include <GameObject.hpp>
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
			// we've filled up the vector, find a dead object and replace it

			std::size_t nextIndex = m_currentObjIndex;
			while (m_objects[nextIndex].isAlive())
			{
				nextIndex++;
				if (nextIndex >= m_objects.capacity()) nextIndex = 0;
				if (nextIndex == m_currentObjIndex)
				{
					std::cout << "Error: Space is full" << std::endl;
					return NULL;
				}
			}

			m_objects[nextIndex] = GameObject(this);
			m_currentObjIndex = nextIndex;

			return &(m_objects[nextIndex]);
		}
	}

	void Space::clear()
	{
		for (auto &cont : m_containers)
		{
			cont.second->clear();
		}

		m_objects.clear();
	}

	void Space::hardClear()
	{
		m_containers.clear();
		m_objects.clear();
	}


	Space::Space(std::size_t maxObjects) :
		m_maxObjects(maxObjects),
		m_currentObjIndex(0)
	{
		m_objects.reserve(maxObjects); // Reserve room for all objects now so reallocations won't happen later
	}

	Space::~Space()
	{
	}
}