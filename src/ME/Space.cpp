#include <Space.hpp>
#include <Physics/PrimitiveQueries.hpp>
#include <GameObject.hpp>
#include <Input/KeyboardController.hpp>
#include <Input/MouseController.hpp>
#include <Utility/UpdateLoopComponent.hpp>
//#include <iostream>

namespace me
{
	void Space::handleWindowEvent(const sf::Event &evt)
	{
		switch (evt.type)
		{
		case sf::Event::KeyPressed :
			each<KeyboardController>([&](Component<KeyboardController> &comp)
			{
				if (comp->onKeyPressed) comp->onKeyPressed(evt.key);
			});
			break;
		case sf::Event::KeyReleased :
			each<KeyboardController>([&](Component<KeyboardController> &comp)
			{
				if (comp->onKeyReleased) comp->onKeyReleased(evt.key);
			});
			break;


		case sf::Event::MouseButtonPressed :
			each<MouseController>([&](Component<MouseController> &comp)
			{
				if (comp->onMouseButtonPressed) comp->onMouseButtonPressed(evt.mouseButton);
			});
			break;
		case sf::Event::MouseButtonReleased:
			each<MouseController>([&](Component<MouseController> &comp)
			{
				if (comp->onMouseButtonReleased) comp->onMouseButtonReleased(evt.mouseButton);
			});
			break;
		case sf::Event::MouseMoved:
			each<MouseController>([&](Component<MouseController> &comp)
			{
				if (comp->onMouseMoved) comp->onMouseMoved(evt.mouseMove);
			});
			break;
		case sf::Event::MouseWheelScrolled:
			each<MouseController>([&](Component<MouseController> &comp)
			{
				if (comp->onMouseWheelScrolled) comp->onMouseWheelScrolled(evt.mouseWheelScroll);
			});
			break;
		}
	}

	void Space::continuousUpdate(sf::Time timeElapsed)
	{
		for (auto &sys : m_systems)
		{
			sys.second->continuousUpdate(timeElapsed);
		}

		each<ContinuousUpdateLoop>([timeElapsed](Component<ContinuousUpdateLoop> &comp)
		{
			comp->update(timeElapsed);
		});
	}

	void Space::fixedUpdate()
	{
		for (auto &sys : m_systems)
		{
			sys.second->fixedUpdate();
		}

		each<FixedUpdateLoop>([](Component<FixedUpdateLoop> &comp)
		{
			comp->update();
		});
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
		if (m_size < m_capacity)
		{
			m_alloc_traits.construct(m_allocator, &(m_objects[m_size]), std::forward<Space*>(this));
			return &(m_objects[m_size++]);
		}
		else
		{
			// we've filled up the array, find a dead object and replace it

			std::size_t nextIndex = m_currentObjIndex;
			while (m_objects[nextIndex].isAlive())
			{
				nextIndex++;
				if (nextIndex >= m_capacity) nextIndex = 0;
				if (nextIndex == m_currentObjIndex)
				{
					std::cerr << "Error: Space is full" << std::endl;
					return NULL;
				}
			}

			m_alloc_traits.destroy(m_allocator, &(m_objects[nextIndex]));
			m_alloc_traits.construct(m_allocator, &(m_objects[nextIndex]), std::forward<Space*>(this));
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

		m_alloc_traits.deallocate(m_allocator, m_objects, m_capacity);
		m_objects = m_alloc_traits.allocate(m_allocator, m_capacity);
		m_size = 0;
		m_currentObjIndex = 0;
	}

	void Space::hardClear()
	{
		m_containers.clear();
		
		m_alloc_traits.deallocate(m_allocator, m_objects, m_capacity);
		m_objects = m_alloc_traits.allocate(m_allocator, m_capacity);
		m_size = 0;
		m_currentObjIndex = 0;
	}


	Space::Space(std::size_t capacity) :
		m_size(0),
		m_capacity(capacity),
		m_currentObjIndex(0)
	{
		m_objects = m_alloc_traits.allocate(m_allocator, capacity);
	}

	Space::~Space()
	{
	}
}