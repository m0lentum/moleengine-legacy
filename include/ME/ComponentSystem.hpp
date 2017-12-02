#ifndef COMPONENT_SYSTEM_HPP
#define COMPONENT_SYSTEM_HPP


// Everything here is adapted and simplified from ECS by Sam Bloomberg (https://github.com/redxdev/ECS)

namespace me
{
	/// A class to allow access to Components through a pointer-like interface.
	template<typename T>
	class ComponentHandle
	{
	private:

		T* m_component;

	public:

		T* operator->() const
		{
			return m_component;
		}

		operator bool() const
		{
			return isValid();
		}

		T& get()
		{
			return *m_component;
		}

		bool isValid() const
		{
			return m_component != nullptr;
		}

		ComponentHandle() :
			m_component(nullptr)
		{
		}

		ComponentHandle(T* m_component) :
			m_component(m_component)
		{
		}

		~ComponentHandle() {}
	};
}

/// Stuff in the Internal namespace should not be used outside of the engine
namespace Internal
{
	class ComponentContainerBase
	{
	public:
		// Nothing here - we just need this as a non-template base to have pointers to.
		ComponentContainerBase() {}
		virtual ~ComponentContainerBase() {}
	};

	template <typename T>
	class ComponentContainer : public ComponentContainerBase
	{
	private:

		std::unique_ptr<T> m_data;

	public:

		T* getData() { return m_data.get(); }

		ComponentContainer() {}
		ComponentContainer(T* data) : m_data(data) {}

		virtual ~ComponentContainer() {}
	};
}


#endif // COMPONENT_SYSTEM_HPP