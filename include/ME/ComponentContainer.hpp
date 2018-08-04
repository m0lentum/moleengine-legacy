#ifndef COMPONENT_CONTAINER_HPP
#define COMPONENT_CONTAINER_HPP

#include <vector>
#include "Component.hpp"
#include <functional>
#include <iostream>
#include <algorithm>
#include <memory>

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

		std::allocator_traits<std::allocator<Component<T>>> m_alloc_traits;
		std::allocator<Component<T>> m_allocator;

		Component<T>* m_components;
		std::size_t m_size;
		const std::size_t m_capacity;
		std::size_t m_currentIndex;

	public:

		template <typename... Args>
		Component<T>* createComponent(GameObject *parent, Args&&... args)
		{
			if (m_size < m_capacity)
			{
				 m_alloc_traits.construct(m_allocator, &(m_components[m_size]), std::forward<GameObject*>(parent), std::forward<Args>(args)...);

				return &(m_components[m_size++]);
			}
			else
			{
				// we've filled up the array, find a dead component and replace it

				std::size_t nextIndex = m_currentIndex;
				while (m_components[nextIndex].isAlive())
				{
					nextIndex++;
					if (nextIndex >= m_size) nextIndex = 0;
					if (nextIndex == m_currentIndex)
					{
						std::cerr << "Error: Container for " << typeid(T).name() << " is full" << std::endl;
						return NULL;
					}
				}

				m_alloc_traits.destroy(m_allocator, &(m_components[nextIndex]));
				m_alloc_traits.construct(m_allocator, &(m_components[nextIndex]), std::forward<GameObject*>(parent), std::forward<Args>(args)...);
				m_currentIndex = nextIndex;

				return &(m_components[nextIndex]);
			}
		}

		/// Execute a function on every (alive) Component in the Container
		void each(std::function<void(Component<T>&)> function)
		{
			for (int i = 0; i < m_size; i++)
			{
				if (m_components[i].isEnabled() && m_components[i].isAlive())
				{
					function(m_components[i]);
				}
			}
		}
		
		/// Delete everything in the Container.
		virtual void clear()
		{
			// just reallocate the whole thing (is this better than destroying everything in place?)
			m_alloc_traits.deallocate(m_allocator, m_components, m_capacity);
			m_components = m_alloc_traits.allocate(m_allocator, m_capacity);
			m_size = 0;
			m_currentIndex = 0;
		}


		ComponentContainer(int capacity) :
			m_currentIndex(0),
			m_size(0),
			m_capacity(capacity)
		{
			m_components = m_alloc_traits.allocate(m_allocator, capacity);
		}

		~ComponentContainer()
		{
			m_alloc_traits.deallocate(m_allocator, m_components, m_capacity);
		}
	};
}

#endif // COMPONENT_CONTAINER_HPP