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
#include "Space.hpp"
#include "Component.hpp"
#include <iostream>

namespace me
{
	class GameObject : public sf::Transformable
	{
	private:
		/// The Space this object is in.
		Space *m_space;

		/// A unique identification number.
		unsigned int m_id;
		/// Tracks how many game objects have been created. Used for generating a unique ID.
		static unsigned int numExisting;

		std::unordered_map<std::type_index, ComponentBase*> m_components;
		bool m_isAlive;

	public:

		void destroy();
		inline bool isAlive() { return m_isAlive; }

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
		void registerComponent(Component<T> *m_component);


		
		GameObject(Space *space);
		GameObject(GameObject &copy) = delete;
		GameObject(GameObject &&move);
		GameObject& operator=(GameObject &&other);
		~GameObject();
	};




	template <typename T, typename ...Args>
	T* GameObject::addComponent(Args&&... args)
	{
		Component<T>* container = m_space->createComponent<T>(this, args...);
		registerComponent<T>(container);
		return container->getComponent();
	}

	template <typename T>
	void GameObject::removeComponent()
	{
		std::type_index index(typeid(T));
		
		if (m_components.count(index) > 0)
		{
			m_components.at(index)->m_isAlive = false;
			m_components.erase(index);
		}
	}

	template <typename T>
	T* GameObject::getComponent()
	{
		std::type_index index(typeid(T));
		
		if (m_components.count(index) > 0)
		{
			return reinterpret_cast<Component<T>*>(m_components.at(index))->getComponent();
		}

		return NULL;
	}

	template <typename T>
	void GameObject::registerComponent(Component<T> *m_component)
	{
		std::type_index index(typeid(T));

		if (m_components.count(index) > 0) // destroy previous m_component of same type if one existed
		{
			m_components.at(index)->m_isAlive = false;
		}

		m_components[index] = m_component;
	}
}

#endif //GAME_OBJECT_HPP