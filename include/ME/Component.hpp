#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <utility>

namespace me
{
	class GameObject;

	// base class for pointer storage in GameObjects
	class ComponentBase
	{
	protected:
		// you can only kill a Component through its parent object
		friend class GameObject;

		GameObject *m_parent;
		bool m_isAlive;

	public:

		inline bool isAlive() const { return m_isAlive; }
		inline GameObject * getParent() const { return m_parent; }

		ComponentBase(GameObject *m_parent) :
			m_parent(m_parent),
			m_isAlive(true)
		{
		}

		ComponentBase(ComponentBase &copy) = delete;

		ComponentBase(ComponentBase&& move) :
			m_parent(move.m_parent),
			m_isAlive(move.m_isAlive)
		{
		}

		virtual ~ComponentBase() {}
	};

	template <typename T>
	class Component : public ComponentBase
	{
	private:

		T m_component;

	public:

		inline T* getComponent() { return &m_component; }


		template <typename... Args>
		Component(GameObject *m_parent, Args&&... args) :
			ComponentBase(m_parent),
			m_component(args...)
		{
		}

		Component(Component<T> &copy) = delete;

		Component(Component<T>&& move) :
			ComponentBase(std::forward<Component<T> >(move)),
			m_component(std::move(move.m_component))
		{
		}

		Component<T>& operator=(Component<T>&& other)
		{
			if (this != &other)
			{
				m_component = std::move(other.m_component);
				m_parent = other.m_parent;
				m_isAlive = other.m_isAlive;
			}

			return *this;
		}

		virtual ~Component() {}
	};
}


#endif // COMPONENT_HPP