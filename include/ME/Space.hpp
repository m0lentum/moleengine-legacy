#ifndef SPACE_HPP
#define SPACE_HPP

#include <vector>
#include <memory>
#include <unordered_map>
#include "ISystem.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include "ComponentContainer.hpp"
#include "Component.hpp"
#include <typeindex>
#include <functional>

namespace me
{
	class GameObject;

	/// A container class for GameObjects. Used to group objects and share them between game states.
	class Space
	{
	private:

		std::size_t m_maxObjects;

		std::unordered_map<std::type_index, std::unique_ptr<ISystem> > m_systems;
		std::unordered_map<std::type_index, std::unique_ptr<ComponentContainerBase> > m_containers;
		std::vector<GameObject> m_objects;

		std::size_t m_currentObjIndex;

	public:

		void continuousUpdate(sf::Time timeElapsed);
		void fixedUpdate();
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;


		GameObject * createObject();

		template <typename T, typename... Args>
		Component<T>* createComponent(GameObject *parent, Args&&... args);


		template <typename T>
		void createContainer();

		template <typename T>
		void createContainer(std::size_t maxSize);

		/// Execute a function on every Component of the given type.
		template <typename T>
		void each(std::function<void(Component<T>&)> function);

		/// Delete all GameObjects and Components but keep Containers
		void clear();
		/// Delete all Containers
		void hardClear();


		template <typename T, typename... Args>
		typename std::enable_if<std::is_base_of<ISystem, T>::value, T*>::type
		createSystem(Args&&... args);

		template <typename T>
		T* getSystem();


	private:

		template <typename T>
		ComponentContainer<T>* getContainer();

	public:

		Space(std::size_t maxObjects);
		~Space();
	};




	

	template <typename T, typename... Args>
	Component<T>* Space::createComponent(GameObject *parent, Args&&... args)
	{
		ComponentContainer<T> *container = getContainer<T>();
		return container->createComponent(parent, args...);
	}

	template <typename T>
	ComponentContainer<T>* Space::getContainer()
	{
		std::type_index index(typeid(T));

		if (m_containers.count(index) == 0)
		{
			createContainer<T>();
		}

		return reinterpret_cast<ComponentContainer<T>*>(m_containers.at(index).get());
	}

	template <typename T>
	void Space::createContainer()
	{
		createContainer<T>(m_maxObjects);
	}

	template <typename T>
	void Space::createContainer(std::size_t maxSize)
	{
		std::type_index index(typeid(T));
		m_containers[index] = std::make_unique<ComponentContainer<T> >(maxSize);
	}

	template <typename T>
	void Space::each(std::function<void(Component<T>&)> function)
	{
		ComponentContainer<T>* container = getContainer<T>();
		if (container) container->each(function);
	}



	template <typename T, typename... Args>
	typename std::enable_if<std::is_base_of<ISystem, T>::value, T*>::type
		Space::createSystem(Args&&... args)
	{
		std::type_index index(typeid(T));
		m_systems[index] = std::make_unique<T>(args...);

		ISystem *created = m_systems[index].get();
		created->registerSpace(this);

		return reinterpret_cast<T*>(created);
	}

	template <typename T>
	T* Space::getSystem()
	{
		std::type_index index(typeid(T));
		if (m_systems.count(index) > 0)
		{
			return m_systems.at(index);
		}

		return NULL;
	}
}


#endif //SPACE_HPP