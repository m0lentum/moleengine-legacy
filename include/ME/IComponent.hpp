#ifndef ICOMPONENT_HPP
#define ICOMPONENT_HPP

#include <string>
#include "GameObject.hpp"

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
		// The ID of the GameObject containing this Component
		inline const unsigned int getID() const { return m_parent->getID(); }

		/// Each Component has a type that is used to identify the Controller it is related to.
		virtual std::string getType() const = 0;


		virtual IComponent * clone() const = 0;

		virtual ~IComponent() {}
	};
}

#endif // ICOMPONENT_HPP