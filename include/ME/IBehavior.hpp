#ifndef IBEHAVIOR_HPP
#define IBEHAVIOR_HPP

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include "GameObject.hpp"

namespace me
{
	/// A Behavior is a self-contained piece of functionality that does not require
	/// knowledge of any other Behavior. It can contain data but the data cannot be accessed
	/// from outside of the Behavior. Used for i.e. graphics and input handling.
	class IBehavior
	{
	protected:

		GameObject *m_parent;

	public:

		inline void registerParent(GameObject *parent) { m_parent = parent; }

		// Update loops. Implement the ones you need
		virtual void continuousUpdate(const sf::Time &timeElapsed) {}
		virtual void fixedUpdate() {}
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {}

		
		virtual IBehavior * clone() const = 0;

		virtual ~IBehavior() {}
	};
}

#endif // IBEHAVIOR_HPP