#ifndef COMPONENT_CONTAINER_HPP
#define COMPONENT_CONTAINER_HPP

#include <vector>
#include "Component.hpp"
#include <functional>
#include <iostream>
#include <algorithm>

namespace me
{
	class GameObject;

	// Base class for containers so we can store references to them
	class ComponentContainerBase
	{
	public:
		virtual void clear() = 0;

		virtual ~ComponentContainerBase() {}
	};


	template <typename T>
	class ComponentContainer : public ComponentContainerBase
	{
	private:

		std::vector<Component<T> > m_components;
		std::size_t m_currentIndex;

	public:

		template <typename... Args>
		Component<T>* createComponent(GameObject *parent, Args&&... args)
		{
			if (m_components.size() < m_components.capacity())
			{
				m_components.emplace_back(parent, args...);

				return &(m_components.back());
			}
			else
			{
				// we've filled up the vector, find a dead component and replace it

				std::size_t nextIndex = m_currentIndex;
				while (m_components[nextIndex].isAlive())
				{
					nextIndex++;
					if (nextIndex >= m_components.capacity()) nextIndex = 0;
					if (nextIndex == m_currentIndex)
					{
						std::cout << "Error: Container for " << typeid(T).name() << " is full" << std::endl;
						return NULL;
					}
				}

				m_components[nextIndex] = Component<T>(parent, args...);
				m_currentIndex = nextIndex;

				return &(m_components[nextIndex]);
			}
		}

		/// Execute a function on every (alive) Component in the Container
		void each(std::function<void(Component<T>&)> function)
		{
			for (auto &unit : m_components)
			{
				if (unit.isEnabled() && unit.isAlive())
				{
					function(unit);
				}
			}
		}
		
		/// Delete everything in the Container.
		virtual void clear()
		{
			m_components.clear();
		}


		ComponentContainer(std::size_t maxSize) :
			m_currentIndex(0)
		{
			m_components.reserve(maxSize);
		}

		~ComponentContainer()
		{
		}
	};
}

#endif // COMPONENT_CONTAINER_HPP