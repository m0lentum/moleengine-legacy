#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <initializer_list>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include "ComponentStorageUnit.hpp"
#include <iostream>

namespace me
{
	class Space;

	class GameObject : public sf::Transformable
	{
	private:
		/// The Space this object is in.
		Space *m_space;

		/// A unique identification number.
		const unsigned int m_id;
		/// Tracks how many game objects have been created. Used for generating a unique ID.
		static unsigned int numExisting;

		std::unordered_map<std::type_index, ComponentStorageUnitBase*> m_components;

	public:

		/// Mark this object for destruction.
		/// It will be removed from the space it's in on the next fixedUpdate cycle.
		void destroy();

		inline const unsigned int getID() const { return m_id; }


		/// Add a Component. It can be of any data type.
		template <typename T, typename ...Args>
		T* addComponent(Args&&... args);

		/// Mark a Component as dead. This does not actually delete it (that is done in
		/// ComponentContainer::cleanup() after marking), but stops it from being updated.
		template <typename T>
		void removeComponent();

		template <typename T>
		T* getComponent();


		/// Used by ComponentContainer to send info back to the object
		template <typename T>
		void registerComponent(ComponentStorageUnit<T> *m_component);


		
		GameObject(Space *space);

		~GameObject();
	};




	template <typename T, typename ...Args>
	T* GameObject::addComponent(Args&&... args)
	{
		return m_space->createComponent<T>(this, args...);
	}

	template <typename T>
	void GameObject::removeComponent()
	{
		std::type_index index(typeid(T));
		
		if (m_components.count(index) > 0)
		{
			m_components.at(index)->m_isAlive = false;
		}
	}

	template <typename T>
	T* GameObject::getComponent()
	{
		std::type_index index(typeid(T));
		
		if (m_components.count(index) > 0)
		{
			return &(reinterpret_cast<ComponentStorageUnit<T>*>(m_components.at(index))->m_component);
		}

		return NULL;
	}

	template <typename T>
	void GameObject::registerComponent(ComponentStorageUnit<T> *m_component)
	{
		std::type_index index(typeid(T));

		if (m_components.count(index) > 0) // destroy previous m_component of same type if one existed
		{
			m_components.at(index)->m_isAlive = false;
			std::cout << "ass" << std::endl;
		}

		m_components[index] = m_component;
	}
}

#endif //GAME_OBJECT_HPP