#ifndef COMPONENT_CONTAINER_HPP
#define COMPONENT_CONTAINER_HPP

#include <vector>
#include "GameObject.hpp"
#include "ComponentStorageUnit.hpp"

namespace me
{
	template <typename T>
	class ComponentContainer
	{
	private:

		std::vector<ComponentStorageUnit<T> > m_components;

	public:

		template <typename... Args>
		T* createComponent(GameObject *parent, Args&&... args)
		{
			m_components.push_back(StorageUnit(parent, T(args...)));
			return &(m_components.back().component);
		}

		void destroyComponent(GameObject *parent);

		ComponentContainer() {}
		~ComponentContainer() {}
	};
}

#endif // COMPONENT_CONTAINER_HPP