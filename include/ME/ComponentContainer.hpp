#ifndef COMPONENT_CONTAINER_HPP
#define COMPONENT_CONTAINER_HPP

#include <vector>
#include "GameObject.hpp"
#include "ComponentStorageUnit.hpp"

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
			m_components.push_back(ComponentStorageUnit<T>(parent, args...));
			ComponentStorageUnit<T> &unit = m_components.back();
			unit.parent->registerComponent<T>(&unit);
			return &(unit.component);
		}
		
		void cleanup();



		inline unsigned int getSize() { return m_components.size(); }


		ComponentContainer() {}
		virtual ~ComponentContainer() {}
	};
}

#endif // COMPONENT_CONTAINER_HPP