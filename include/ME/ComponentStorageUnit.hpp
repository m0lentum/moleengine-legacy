#ifndef COMPONENT_STORAGE_UNIT_HPP
#define COMPONENT_STORAGE_UNIT_HPP

#include <utility>

namespace me
{
	class GameObject;

	// base class for pointer storage in GameObjects
	class ComponentStorageUnitBase
	{
	protected:
		// allow objects to manipulate stuff directly, others can only read
		friend class GameObject;

		GameObject *m_parent;
		bool m_isAlive;

	public:

		inline bool isAlive() const { return m_isAlive; }
		inline GameObject * getParent() const { return m_parent; }

		ComponentStorageUnitBase(GameObject *m_parent) :
			m_parent(m_parent),
			m_isAlive(true)
		{
		}

		ComponentStorageUnitBase(ComponentStorageUnitBase &copy) = delete;

		ComponentStorageUnitBase(ComponentStorageUnitBase&& move) :
			m_parent(move.m_parent),
			m_isAlive(move.m_isAlive)
		{
		}

		virtual ~ComponentStorageUnitBase() {}
	};

	template <typename T>
	class ComponentStorageUnit : public ComponentStorageUnitBase
	{
	private:

		T m_component;

	public:

		inline T* getComponent() { return &m_component; }

		inline void remove() { m_parent->removeComponent<T>(); }

		template <typename... Args>
		ComponentStorageUnit(GameObject *m_parent, Args&&... args) :
			ComponentStorageUnitBase(m_parent),
			m_component(args...)
		{
		}

		ComponentStorageUnit(ComponentStorageUnit<T> &copy) = delete;

		ComponentStorageUnit(ComponentStorageUnit<T>&& move) :
			ComponentStorageUnitBase(std::forward<ComponentStorageUnit<T> >(move)),
			m_component(std::move(move.m_component))
		{
		}

		ComponentStorageUnit<T>& operator=(ComponentStorageUnit<T>&& other)
		{
			if (this != &other)
			{
				m_component = std::move(other.m_component);
				m_parent = other.m_parent;
				m_isAlive = other.m_isAlive;
			}

			return *this;
		}

		virtual ~ComponentStorageUnit() {}
	};
}


#endif // COMPONENT_STORAGE_UNIT_HPP