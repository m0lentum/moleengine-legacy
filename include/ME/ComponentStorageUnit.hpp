#ifndef COMPONENT_STORAGE_UNIT_HPP
#define COMPONENT_STORAGE_UNIT_HPP

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


		virtual ~ComponentStorageUnit() {}
	};
}


#endif // COMPONENT_STORAGE_UNIT_HPP