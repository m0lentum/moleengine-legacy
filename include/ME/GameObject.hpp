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
#include "ComponentSystem.hpp"

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

		std::unordered_map<std::type_index, std::unique_ptr<Internal::ComponentContainerBase> > m_components;

	public:

		/// Mark this object for destruction.
		/// It will be removed from the space it's in on the next fixedUpdate cycle.
		void destroy();

		inline const unsigned int getID() const { return m_id; }


		/// Add a Component. It can be of any data type.
		template <typename T>
		ComponentHandle<T> addComponent(T* m_component);

		template <typename T>
		void removeComponent();

		template <typename T>
		ComponentHandle<T> getComponent();

		
		GameObject(Space *space);

		~GameObject();
	};




	template <typename T>
	ComponentHandle<T> GameObject::addComponent(T* m_component)
	{
		std::type_index index(typeid(T)); // Get index from type

		m_components.emplace(index, std::make_unique<Internal::ComponentContainer<T> >(m_component)); // put in the map

		return ComponentHandle<T>(m_component);
	}

	template <typename T>
	void GameObject::removeComponent()
	{
		std::type_index index(typeid(T));

		m_components.erase(index);
	}

	template <typename T>
	ComponentHandle<T> GameObject::getComponent()
	{
		std::type_index index(typeid(T));

		if (m_components.count(index) > 0)
		{
			Internal::ComponentContainer<T>* container = reinterpret_cast<Internal::ComponentContainer<T>*>(m_components.at(index).get());
			return ComponentHandle<T>(container->getData());
		}
		else
		{
			return ComponentHandle<T>(); // Return invalid handle if not found
		}
	}
}

#endif //GAME_OBJECT_HPP