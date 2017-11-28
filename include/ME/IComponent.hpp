#ifndef ICOMPONENT_HPP
#define ICOMPONENT_HPP

#include <string>

namespace me
{
	class GameObject;

	/// Components are used to add functionality to GameObjects in a modular fashion and controlled by Controllers.
	/// In order for this to work the GameObject must be stored inside a Space.
	class IComponent
	{
	protected:

		GameObject *m_parent;

	public:

		inline void registerParent(GameObject *parent) { m_parent = parent; }

		/// Each Component has a type that is used to identify the Controller it is related to.
		virtual std::string getType() = 0;


		virtual IComponent * clone() = 0;

		virtual ~IComponent() {}
	};
}

#endif // ICOMPONENT_HPP