#include <Space.hpp>
#include <Physics/CollisionChecker.hpp>
#include <GameObject.hpp>
//#include <iostream>

namespace me
{
	void Space::continuousUpdate(sf::Time timeElapsed)
	{
		for (auto &sys : m_systems)
		{
			sys.second->continuousUpdate(timeElapsed);
		}
	}

	void Space::fixedUpdate()
	{
		for (auto &sys : m_systems)
		{
			sys.second->fixedUpdate();
		}
	}

	void Space::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		for (auto &sys : m_systems)
		{
			sys.second->draw(target, states);
		}
	}

	GameObject * Space::createObject()
	{
		if (m_objects.size() < m_objects.capacity())
		{
			m_objects.emplace_back(this);
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