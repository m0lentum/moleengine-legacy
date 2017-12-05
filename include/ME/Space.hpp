#ifndef SPACE_HPP
#define SPACE_HPP

#include <vector>
#include <memory>
#include <unordered_map>
#include "GameObject.hpp"
#include "IController.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include "ComponentContainer.hpp"
#include "ComponentStorageUnit.hpp"
#include <typeindex>

namespace me
{
	/// A container class for GameObjects. Used to group objects and share them between game states.
	class Space
	{
	private:

		std::vector<std::unique_ptr<IController> > m_controllers;
		std::unordered_map<std::type_index, std::unique_ptr<ComponentContainerBase> > m_containers;
		std::vector<GameObject> m_objects;

	public:

		void continuousUpdate(sf::Time timeElapsed);
		void fixedUpdate();
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;


		void addController(IController *controller);

		GameObject * createObject();

		template <typename T, typename... Args>
		T* createComponent(GameObject *parent, Args&&... args);


		Space();
		Space(const Space &copy);
		~Space();

	private:

		template <typename T>
		void createContainer();
	public:
		template <typename T>
		ComponentContainer<T>* getContainer();
	};




	template <typename T, typename... Args>
	T* Space::createComponent(GameObject *parent, Args&&... args)
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
		std::type_index index(typeid(T));
		m_containers[index] = std::make_unique<ComponentContainer<T> >();
	}
}


#endif //SPACE_HPP