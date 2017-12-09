#ifndef ICONTROLLER_HPP
#define ICONTROLLER_HPP

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace me
{
	class Space;

	/// An interface for classes that control various Components.
	class ISystem
	{
	protected:

		Space *m_space;

	public:

		virtual void continuousUpdate(sf::Time timeElapsed) {}
		virtual void fixedUpdate() {}
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const {}


		inline void registerSpace(Space *space) { m_space = space; }
		virtual ~ISystem() {}
	};
}

#endif // ICONTROLLER_HPP