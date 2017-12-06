#ifndef COMPONENT_CONTAINER_HPP
#define COMPONENT_CONTAINER_HPP

#include <vector>
#include "ComponentStorageUnit.hpp" 
#include "GameObject.hpp"
#include <functional>
#include <iostream>
#include <algorithm>

namespace me
{
	// Base class for containers so we can store references to them
	class ComponentContainerBase
	{
	public:
		virtual ~ComponentContainerBase() {}
	};


	template <typename T>
	class ComponentContainer : public ComponentContainerBase
	{
	private:

		std::vector<ComponentStorageUnit<T> > m_components;

	public:

		template <typename... Args>
		T* createComponent(GameObject *parent, Args&&... args)
		{
			if (m_components.size() < m_components.max_size())
			{
				m_components.push_back(ComponentStorageUnit<T>(parent, args...));
				ComponentStorageUnit<T> &unit = m_components.back();
				parent->registerComponent<T>(&unit);
				return unit.getComponent();
			}
			else
			{
				std::cout << "Error: ComponentContainer is full" << std::endl;
				return NULL;
			}
		}
		
		/// Remove all components that have been marked dead
		void cleanup()
		{
			m_components.erase(std::remove_if(m_components.begin(), m_components.end(),
				[](ComponentStorageUnit<T> &unit) { return !unit.isAlive(); }),
				m_components.end());

			// IMPORTANT: this currently breaks pointers! Fix this!
		}

		/// Execute a function on every (alive) Component in the Container
		void each(std::function<void(ComponentStorageUnit<T>&)> function)
		{
			for (auto &unit : m_components)
			{
				if (unit.isAlive())
				{
					function(unit);
				}
			}
		}

		inline unsigned int getSize() { return m_components.size(); }


		ComponentContainer(std::size_t maxSize) { m_components.reserve(maxSize); }
		~ComponentContainer() {}
	};
}

#endif // COMPONENT_CONTAINER_HPP